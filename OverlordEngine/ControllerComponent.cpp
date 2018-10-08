#include "stdafx.h"
#include "ControllerComponent.h"
#include "TransformComponent.h"
#include "PhysxProxy.h"
#include "GameObject.h"
#include "GameScene.h"

ControllerComponent::ControllerComponent(physx::PxMaterial* material, float radius, float height, std::wstring name,
                                         physx::PxCapsuleClimbingMode::Enum climbingMode)
	: m_Radius(radius),
	  m_Height(height),
	  m_Name(std::move(name)),
	  m_Controller(nullptr),
	  m_ClimbingMode(climbingMode),
	  m_pMaterial(material),
	  m_CollisionFlag(physx::PxControllerCollisionFlags()),
	  m_CollisionGroups(physx::PxFilterData(CollisionGroupFlag::Group0, 0, 0, 0))
{
}

void ControllerComponent::Initialize(const GameContext&)
{
	if (m_Controller != nullptr)
	{
		Logger::LogError(L"[ControllerComponent] Cannot initialize a controller twice");
		return;
	}

	auto manager = GetGameObject()->GetScene()->GetPhysxProxy()->GetControllerManager();
	physx::PxCapsuleControllerDesc desc;
	desc.setToDefault();
	desc.radius = m_Radius;
	desc.height = m_Height;
	desc.climbingMode = m_ClimbingMode;
	desc.upDirection = physx::PxVec3(0, 1, 0);
	desc.contactOffset = 0.1f;

	const auto position = GetGameObject()->GetTransform()->GetPosition();
	desc.position = ToPxExtendedVec3(position);
	desc.material = m_pMaterial;
	desc.userData = this;

	m_Controller = manager->createController(desc);

	if (m_Controller == nullptr)
	{
		Logger::LogError(L"[ControllerComponent] Failed to create controller");
		return;
	}

	std::string name(m_Name.begin(), m_Name.end());
	m_Controller->getActor()->setName(name.c_str());

	m_Controller->getActor()->userData = this;
	SetCollisionGroup(static_cast<CollisionGroupFlag>(m_CollisionGroups.word0));
	SetCollisionIgnoreGroups(static_cast<CollisionGroupFlag>(m_CollisionGroups.word1));
}

void ControllerComponent::Update(const GameContext&)
{
}

void ControllerComponent::Draw(const GameContext&)
{
}

void ControllerComponent::Translate(const DirectX::XMFLOAT3& position) const
{
	Translate(position.x, position.y, position.z);
}

void ControllerComponent::Translate(const float x, const float y, const float z) const
{
	if (m_Controller == nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot Translate. No controller present");
	else
		m_Controller->setPosition(physx::PxExtendedVec3(x, y, z));
}

void ControllerComponent::Move(const DirectX::XMFLOAT3 displacement, const float minDist)
{
	if (m_Controller == nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot Move. No controller present");
	else
		m_CollisionFlag = m_Controller->move(ToPxVec3(displacement), minDist, 0, nullptr, nullptr);
}

DirectX::XMFLOAT3 ControllerComponent::GetPosition() const
{
	if (m_Controller == nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot get position. No controller present");
	else
		return ToXMFLOAT3(m_Controller->getPosition());

	return DirectX::XMFLOAT3();
}

DirectX::XMFLOAT3 ControllerComponent::GetFootPosition() const
{
	if (m_Controller == nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot get footposition. No controller present");
	else
		return ToXMFLOAT3(m_Controller->getFootPosition());

	return DirectX::XMFLOAT3();
}

void ControllerComponent::SetCollisionIgnoreGroups(const CollisionGroupFlag ignoreGroups)
{
	using namespace physx;

	m_CollisionGroups.word1 = ignoreGroups;

	if (m_Controller != nullptr)
	{
		const auto actor = m_Controller->getActor();
		const auto numShapes = actor->getNbShapes();
		const auto shapes = new PxShape*[numShapes];

		const auto numPointers = actor->getShapes(shapes, numShapes);
		for (PxU32 i = 0; i < numPointers; i++)
		{
			auto shape = shapes[i];
			shape->setSimulationFilterData(m_CollisionGroups);
			//TODO: shouldn't the query filter data be set as well?
		}
		delete[] shapes;
	}
}

void ControllerComponent::SetCollisionGroup(const CollisionGroupFlag group)
{
	using namespace physx;

	m_CollisionGroups.word0 = group;

	if (m_Controller != nullptr)
	{
		const auto actor = m_Controller->getActor();
		const auto numShapes = actor->getNbShapes();
		const auto shapes = new PxShape*[numShapes];

		const auto numPointers = actor->getShapes(shapes, numShapes);
		for (PxU32 i = 0; i < numPointers; i++)
		{
			auto shape = shapes[i];
			shape->setSimulationFilterData(m_CollisionGroups);
			shape->setQueryFilterData(m_CollisionGroups);
		}
		delete[] shapes;
	}
}
