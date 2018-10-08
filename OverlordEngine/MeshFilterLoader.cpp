#include "stdafx.h"
#include "MeshFilterLoader.h"
#include "BinaryReader.h"
#include "EffectHelper.h"

#define OVM_vMAJOR 1
#define OVM_vMINOR 1

MeshFilter* MeshFilterLoader::LoadContent(const std::wstring& assetFile)
{
	auto binReader = new BinaryReader();
	binReader->Open(assetFile);

	if(!binReader->Exists())
		return nullptr;

	//READ OVM FILE
	const int versionMajor = binReader->Read<char>();
	const int versionMinor = binReader->Read<char>();

	if (versionMajor != OVM_vMAJOR || versionMinor != OVM_vMINOR)
	{
		Logger::LogFormat(LogLevel::Warning, L"MeshDataLoader::Load() > Wrong OVM version\n\tFile: \"%s\" \n\tExpected version %i.%i, not %i.%i.", assetFile.c_str(), OVM_vMAJOR, OVM_vMINOR, versionMajor, versionMinor);
		delete binReader;
		return nullptr;
	}

	unsigned int vertexCount = 0;
	unsigned int indexCount = 0;

	auto pMesh = new MeshFilter();

	for (;;)
	{
		const auto meshDataType = static_cast<MeshDataType>(binReader->Read<char>());
		if(meshDataType == MeshDataType::END)
			break;

		const auto dataOffset = binReader->Read<unsigned int>();

		switch(meshDataType)
		{
		case MeshDataType::HEADER:
			{
				pMesh->m_MeshName = binReader->ReadString();
				vertexCount = binReader->Read<unsigned int>();
				indexCount = binReader->Read<unsigned int>();

				pMesh->m_VertexCount = vertexCount;
				pMesh->m_IndexCount = indexCount;
			}
			break;
		case MeshDataType::POSITIONS:
			{
				pMesh->m_HasElement |= ILSemantic::POSITION;

				for(unsigned int i = 0; i<vertexCount; ++i)
				{
				 DirectX::XMFLOAT3 pos;
					pos.x = binReader->Read<float>();
					pos.y = binReader->Read<float>();
					pos.z = binReader->Read<float>();
					pMesh->m_Positions.push_back(pos);
				}
			}
			break;
		case MeshDataType::INDICES:
			{
				for(unsigned int i = 0; i<indexCount; ++i)
				{
					pMesh->m_Indices.push_back(binReader->Read<DWORD>());
				}
			}
			break;
		case MeshDataType::NORMALS:
			{
				pMesh->m_HasElement |= ILSemantic::NORMAL;

				for (unsigned int i = 0; i<vertexCount; ++i)
				{
				 DirectX::XMFLOAT3 normal;
					normal.x = binReader->Read<float>();
					normal.y = binReader->Read<float>();
					normal.z = binReader->Read<float>();
					pMesh->m_Normals.push_back(normal);
				}
			}
			break;
		case MeshDataType::TANGENTS:
			{
				pMesh->m_HasElement |= ILSemantic::TANGENT;

				for (unsigned int i = 0; i<vertexCount; ++i)
				{
				 DirectX::XMFLOAT3 tangent;
					tangent.x = binReader->Read<float>();
					tangent.y = binReader->Read<float>();
					tangent.z = binReader->Read<float>();
					pMesh->m_Tangents.push_back(tangent);
				}
			}
			break;
		case MeshDataType::BINORMALS:
			{
				pMesh->m_HasElement |= ILSemantic::BINORMAL;

				for (unsigned int i = 0; i<vertexCount; ++i)
				{
				 DirectX::XMFLOAT3 binormal;
					binormal.x = binReader->Read<float>();
					binormal.y = binReader->Read<float>();
					binormal.z = binReader->Read<float>();
					pMesh->m_Binormals.push_back(binormal);
				}
			}
			break;
		case MeshDataType::TEXCOORDS:
			{
				pMesh->m_HasElement |= ILSemantic::TEXCOORD;

				for (unsigned int i = 0; i<vertexCount; ++i)
				{
				 DirectX::XMFLOAT2 tc;
					tc.x = binReader->Read<float>();
					tc.y = binReader->Read<float>();
					pMesh->m_TexCoords.push_back(tc);
				}
			}
			break;
		case MeshDataType::COLORS:
			{
				pMesh->m_HasElement |= ILSemantic::COLOR;

				for (unsigned int i = 0; i<vertexCount; ++i)
				{
				 DirectX::XMFLOAT4 color;
					color.x = binReader->Read<float>();
					color.y = binReader->Read<float>();
					color.z = binReader->Read<float>();
					color.w = binReader->Read<float>();
					pMesh->m_Colors.push_back(color);
				}
			}
			break;
		case MeshDataType::BLENDINDICES:
			{
				pMesh->m_HasElement |= ILSemantic::BLENDINDICES;

				for (unsigned int i = 0; i<vertexCount; ++i)
				{
				 DirectX::XMFLOAT4 indices;
					indices.x = binReader->Read<float>();
					indices.y = binReader->Read<float>();
					indices.z = binReader->Read<float>();
					indices.w = binReader->Read<float>();
					pMesh->m_BlendIndices.push_back(indices);
				}
			}
			break;
		case MeshDataType::BLENDWEIGHTS:
			{
				pMesh->m_HasElement |= ILSemantic::BLENDWEIGHTS;

				for (unsigned int i = 0; i<vertexCount; ++i)
				{
				 DirectX::XMFLOAT4 weights;
					weights.x = binReader->Read<float>();
					weights.y = binReader->Read<float>();
					weights.z = binReader->Read<float>();
					weights.w = binReader->Read<float>();
					pMesh->m_BlendWeights.push_back(weights);
				}
			}
			break;
		case MeshDataType::ANIMATIONCLIPS:
			{
				pMesh->m_HasAnimations = true;
				const auto clipCount = binReader->Read<USHORT>();

				for (auto j = 0; j < clipCount; ++j)
				{
					AnimationClip clip;
					clip.Name = binReader->ReadString();
					clip.Duration = binReader->Read<float>();
					clip.TicksPerSecond = binReader->Read<float>();

					const auto keyCount = binReader->Read<USHORT>();

					for (auto key = 0; key < keyCount; ++key)
					{
						AnimationKey animKey;
						animKey.Tick = binReader->Read<float>();

						const auto transformCount = binReader->Read<USHORT>();
						for (auto i = 0; i < transformCount; ++i)
						{
						 DirectX::XMFLOAT4X4 matrix;
							matrix._11 = binReader->Read<float>();
							matrix._12 = binReader->Read<float>();
							matrix._13 = binReader->Read<float>();
							matrix._14 = binReader->Read<float>();
							matrix._21 = binReader->Read<float>();
							matrix._22 = binReader->Read<float>();
							matrix._23 = binReader->Read<float>();
							matrix._24 = binReader->Read<float>();
							matrix._31 = binReader->Read<float>();
							matrix._32 = binReader->Read<float>();
							matrix._33 = binReader->Read<float>();
							matrix._34 = binReader->Read<float>();
							matrix._41 = binReader->Read<float>();
							matrix._42 = binReader->Read<float>();
							matrix._43 = binReader->Read<float>();
							matrix._44 = binReader->Read<float>();
							animKey.BoneTransforms.push_back(matrix);
						}

						clip.Keys.push_back(animKey);
					}

					pMesh->m_AnimationClips.push_back(clip);
				}
			}
			break;
		case MeshDataType::SKELETON:
			{
				pMesh->m_BoneCount = binReader->Read<USHORT>();
				binReader->MoveBufferPosition(dataOffset - sizeof(USHORT));
			}
			break;
		default:
			binReader->MoveBufferPosition(dataOffset);
			break;
		}
	}

	delete binReader;

	return pMesh;
}

void MeshFilterLoader::Destroy(MeshFilter* objToDestroy)
{
	SafeDelete(objToDestroy);
}
