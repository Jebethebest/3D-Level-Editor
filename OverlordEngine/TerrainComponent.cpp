#include "stdafx.h"

#include "TerrainComponent.h"
 #include <utility>

#include "ContentManager.h"
#include "EffectHelper.h"
#include "TransformComponent.h"
#include "TextureData.h"
#include "PhysxManager.h"
#include "PhysxProxy.h"
#include "GameObject.h"
#include "GameScene.h"


TerrainComponent::TerrainComponent(std::wstring heightMapFile, std::wstring textureFile,
                                   unsigned int nrOfRows, unsigned int nrOfColumns, float width, float depth,
                                   float maxHeight) :
	m_HeightMapFile(std::move(heightMapFile)),
	m_TextureFile(std::move(textureFile)),
	m_NrOfRows(nrOfRows),
	m_NrOfColumns(nrOfColumns),
	m_NrOfVertices(nrOfRows * nrOfColumns),
	m_Width(width),
	m_Depth(depth),
	m_MaxHeight(maxHeight)
{
}

TerrainComponent::~TerrainComponent()
{
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
}

void TerrainComponent::Initialize(const GameContext& gameContext)
{
	//Load Effect
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"Resources/Effects/PosNormTex3D.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);

	//TODO: Error handling
	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_pMatWorldVariable = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
	m_pDiffuseVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	UINT ilSize = 0;
	if (!EffectHelper::BuildInputLayout(gameContext.pDevice, m_pTechnique, &m_pInputLayout, ilSize))
		Logger::LogError(L"TerrainComponent::Initialize >> BuildInputLayout failed!");

	//Texture
	m_pTextureData = ContentManager::Load<TextureData>(m_TextureFile);

	//Load Height Map
	ReadHeightMap();

	//Create Vertices & Triangles
	CalculateVerticesAndIndices();

	//Build Vertexbuffer
	BuildVertexBuffer(gameContext);

	//Build Indexbuffer
	BuildIndexBuffer(gameContext);

	//Create PhysX Heightfield
	CreatePxHeightField();
}

void TerrainComponent::Update(const GameContext& )
{
}

void TerrainComponent::Draw(const GameContext& gameContext)
{
	DirectX::XMMATRIX world = XMLoadFloat4x4(&GetTransform()->GetWorld());
	const DirectX::XMMATRIX viewProj = XMLoadFloat4x4(&gameContext.pCamera->GetViewProjection());
	DirectX::XMMATRIX wvp = XMMatrixMultiply(world, viewProj);
	m_pMatWorldVariable->SetMatrix(reinterpret_cast<float*>(&world));
	m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<float*>(&wvp));
	m_pDiffuseVariable->SetResource(m_pTextureData->GetShaderResourceView());

	// Set vertex buffer
	const unsigned int stride = sizeof(VertexPosNormTex);
	unsigned int offset = 0;
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set index buffer
	gameContext.pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the input layout
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	// Set primitive topology
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render a cube
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}
}

void TerrainComponent::ReadHeightMap()
{
	//make sure the value has enough space for all the high values
	m_VecHeightValues.resize(m_NrOfVertices, 0);

	//open binary file 
	std::ifstream inFile;
	inFile.open(m_HeightMapFile.c_str(), std::ios_base::binary);
	if (!inFile)
	{
		Logger::LogFormat(LogLevel::Error, L"Loading terrain \'%ls\' failed!", m_HeightMapFile.c_str());
	}

	//read height info
	inFile.read(reinterpret_cast<char*>(&m_VecHeightValues[0]),m_NrOfVertices * sizeof(unsigned short));
	inFile.close();
}

void TerrainComponent::CalculateVerticesAndIndices()
{
	// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	//**VERTICES
	m_VecVertices.reserve(m_NrOfVertices);

	const float cellWidth = m_Width / m_NrOfRows;
	const float cellHeight = m_Depth / m_NrOfColumns;

	float cellZPos = m_Depth / 2.f;

	for (unsigned int row = 0; row < m_NrOfRows; ++row)
	{
		//Reset the cellXPos Position for each Column
		float cellXPos = -m_Width / 2.f;
		for (unsigned int col = 0; col < m_NrOfColumns; ++col)
		{
			auto vertex = VertexPosNormTex();
			//1. Position
			vertex.Position.x = cellXPos;
			vertex.Position.y = m_VecHeightValues[row * m_NrOfColumns + col] * m_MaxHeight / USHRT_MAX;
			vertex.Position.z = cellZPos;

			//2. Normal
			vertex.Normal.x = 0;
			vertex.Normal.y = 1;
			vertex.Normal.z = 0;

			//3. TexCoord
			vertex.TexCoord.x = col / static_cast<float>(m_NrOfColumns - 1);
			vertex.TexCoord.y = row / static_cast<float>(m_NrOfRows - 1);

			m_VecVertices.push_back(vertex);

			if (row < m_NrOfRows - 1 && col < m_NrOfColumns - 1)
			{
				const int a = row * m_NrOfColumns + col;
				const int b = a + 1;
				const int c = a + m_NrOfColumns;
				const int d = c + 1;
				AddQuad(a, b, c, d);
			}

			//Move the cellXPos Position (Down)
			cellXPos += cellWidth;
		}

		//Move the cellZPos Position (Right)
		cellZPos -= cellHeight;
	}

	std::vector<XMFLOAT3> faceNormals;
	for (unsigned int i = 0; i < m_VecIndices.size(); i += 6)
	{
		//Get the positions of 6 vertices
		const auto a = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i)).Position);
		const auto b = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 1)).Position);
		const auto c = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 2)).Position);
		const auto d = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 3)).Position);
		const auto e = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 4)).Position);
		const auto f = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 5)).Position);

		//first triangle
		auto v1 = c - a;
		auto v2 = b - a;
		auto normal = XMVector3Cross(v1, v2);
		normal = XMVector3Normalize(normal);
		XMFLOAT3 normalFloat3 {};
		XMStoreFloat3(&normalFloat3, normal);
		faceNormals.push_back(normalFloat3);

		//second triangle
		v1 = f - e;
		v2 = f - d;
		normal = XMVector3Cross(v1, v2);
		normal = XMVector3Normalize(normal);
		XMStoreFloat3(&normalFloat3, normal);
		faceNormals.push_back(normalFloat3);
	}
	//iterate through the vertices and calculate a normal for each one of them using the average of the 6 adjacent faces
	const int numFacesPerRow = (m_NrOfColumns - 1) * 2;
	int index[6];
	//from left front to right back
	for (unsigned int row = 0; row < m_NrOfRows; ++row)
	{
		for (unsigned int col = 0; col < m_NrOfColumns; ++col)
		{
			const int centerindex = numFacesPerRow * row + col * 2;
			index[0] = centerindex - 1;
			index[1] = centerindex;
			index[2] = centerindex + 1;
			index[3] = centerindex - numFacesPerRow - 2;
			index[4] = centerindex - numFacesPerRow - 1;
			index[5] = centerindex - numFacesPerRow - 0;
			//if col==0 is on left edge, there are 
			//no vertices on the left, fill in a illegal index
			if (col == 0)
			{
				index[0] = -1;
				index[3] = -1;
				index[4] = -1;
			}
			//if col==m_NumColumns-1 is on right edge, there are 
			//no vertices on the right, fill in a illegal index
			if (col == m_NrOfColumns - 1)
			{
				index[1] = -1;
				index[2] = -1;
				index[5] = -1;
			}

			//if index<0 or out of range: front or back edge 
			//it may not be used to calculate the average
			XMVECTOR sum{};
			for (auto i : index)
			{
				if (i >= 0 && i < static_cast<int>(faceNormals.size()))
				{
					sum += XMLoadFloat3(&faceNormals.at(index[i]));
				}
			}
			//calculate average by normalizing 
			sum = XMVector3Normalize(sum);
			XMFLOAT3 sumFloat3 {};
			XMStoreFloat3(&sumFloat3, sum);
			m_VecVertices[row * m_NrOfColumns + col].Normal = sumFloat3;
		}
	}
}

void TerrainComponent::AddTriangle(const unsigned int a, const unsigned int b, const unsigned c)
{
	m_VecIndices.push_back(a);
	m_VecIndices.push_back(b);
	m_VecIndices.push_back(c);
}

void TerrainComponent::AddQuad(const unsigned int a, const unsigned int b, const unsigned c, const unsigned d)
{
	AddTriangle(a, c, d);
	AddTriangle(a, d, b);
}

void TerrainComponent::BuildVertexBuffer(const GameContext& gameContext)
{
	//Vertexbuffer
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = {nullptr};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(VertexPosNormTex) * m_NrOfVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_VecVertices.data();
	const HRESULT hr = gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	Logger::LogHResult(hr, L"Failed to Create Vertexbuffer");
}

void TerrainComponent::BuildIndexBuffer(const GameContext& gameContext)
{
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = {nullptr};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(DWORD) * m_VecIndices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_VecIndices.data();
	const HRESULT hr = gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	Logger::LogHResult(hr, L"Failed to Create Indexbuffer");
	m_NumIndices = m_VecIndices.size();
}

void TerrainComponent::CreatePxHeightField()
{
	physx::PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();
	physx::PxMaterial* defaultMaterial = pPhysX->createMaterial(.5f, .5f, .1f);

	physx::PxHeightFieldSample sample;
	std::vector<physx::PxHeightFieldSample> vecSamples;
	vecSamples.reserve(m_NrOfColumns * m_NrOfRows);

	for (unsigned int row = 0; row < m_NrOfRows; ++row)
	{
		for (unsigned int col = 0; col < m_NrOfColumns; ++col)
		{
			//convert from unsigned short to signed short
			sample.height = m_VecHeightValues.at((m_NrOfColumns - col - 1) * m_NrOfColumns + row) / 2;
			sample.materialIndex0 = 0;
			sample.materialIndex1 = 0;
			sample.setTessFlag();
			vecSamples.push_back(sample);
		}
	}

	physx::PxHeightFieldDesc hfDesc;
	hfDesc.format = physx::PxHeightFieldFormat::eS16_TM;
	hfDesc.nbColumns = m_NrOfColumns;
	hfDesc.nbRows = m_NrOfRows;
	hfDesc.samples.data = vecSamples.data();
	hfDesc.samples.stride = sizeof(physx::PxHeightFieldSample);

	physx::PxHeightField* aHeightField = pPhysX->createHeightField(hfDesc);
	const auto heightScale = 2 * m_MaxHeight / USHRT_MAX;
	const auto rowScale = m_Width / m_NrOfColumns;
	const auto colScale = m_Depth / m_NrOfRows;
	const physx::PxHeightFieldGeometry hfGeom(aHeightField, physx::PxMeshGeometryFlags(), heightScale, rowScale, colScale);
	physx::PxRigidStatic* terrainActor = pPhysX->createRigidStatic(physx::PxTransform::createIdentity());

	physx::PxShape* pTerrainShape = terrainActor->createShape(hfGeom, *defaultMaterial,
	                                                          physx::PxTransform(
		                                                          physx::PxVec3(
			                                                          -m_Width / 2.f, 0.f, -m_Depth / 2.f + colScale)));
	pTerrainShape->setSimulationFilterData(physx::PxFilterData(Group0, 0, 0, 0));

	GetGameObject()->GetScene()->GetPhysxProxy()->GetPhysxScene()->addActor(*terrainActor);
}
