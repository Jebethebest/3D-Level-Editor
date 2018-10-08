
#include "stdafx.h"

#include "ModelAnimator.h"



ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
m_pMeshFilter(pMeshFilter),
m_Transforms(std::vector<DirectX::XMFLOAT4X4>()),
m_IsPlaying(false), 
m_Reversed(false),
m_ClipSet(false),
m_TickCount(0),
m_AnimationSpeed(1.0f)
{
	SetAnimation(0);
}


ModelAnimator::~ModelAnimator()
{
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	m_ClipSet = false;

	if (clipNumber >= m_pMeshFilter->m_AnimationClips.size())
	{
		Reset();
		Logger::LogFormat(LogLevel::Warning, L"ModelAnimator::SetAnimation() > Animation with clipNumber \'%i\' not found!", clipNumber);
		return;
	}
		

	SetAnimation(m_pMeshFilter->m_AnimationClips[clipNumber]);
}

void ModelAnimator::SetAnimation(std::wstring clipName)
{
	m_ClipSet = false;

	for (auto clip : m_pMeshFilter->m_AnimationClips)
	{
		if (wcscmp(clipName.c_str(), clip.Name.c_str()) == 0)
		{
			SetAnimation(clip);
		}
	}

	Reset();
	Logger::LogFormat(LogLevel::Warning, L"ModelAnimator::SetAnimation() > Animation \'%s\' not found!", clipName.c_str());
}

void ModelAnimator::SetAnimation(AnimationClip clip)
{
	m_ClipSet = true;
	m_CurrentClip = clip;
	
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	if (pause)
		m_IsPlaying = false;

	m_TickCount = 0;
	m_AnimationSpeed = 1.0f;

	if (m_ClipSet)
	{
		auto rootTransforms = m_CurrentClip.Keys.begin()->BoneTransforms;
		m_Transforms.assign(rootTransforms.begin(), rootTransforms.end());
	}
	else
	{
	 DirectX::XMFLOAT4X4 identityMatrix;
		XMStoreFloat4x4(&identityMatrix, DirectX::XMMatrixIdentity());
		m_Transforms.assign(m_pMeshFilter->m_BoneCount, identityMatrix);
	}
}

void ModelAnimator::Update(const GameContext& gameContext)
{
	if (m_IsPlaying && m_ClipSet)
	{
		auto passedTicks = gameContext.pGameTime->GetElapsed() * m_CurrentClip.TicksPerSecond * m_AnimationSpeed;
		passedTicks = fmod(passedTicks, m_CurrentClip.Duration);

		if (m_Reversed)
		{
			m_TickCount -= passedTicks;
			if (m_TickCount < 0)
				m_TickCount += m_CurrentClip.Duration;
		}
		else
		{
			m_TickCount += passedTicks;
			if (m_TickCount>m_CurrentClip.Duration)
				m_TickCount -= m_CurrentClip.Duration;
		}

		//Find Current and Next Key
		AnimationKey keyA, keyB;
		auto keyCount = m_CurrentClip.Keys.size();
		for (UINT i = 0; i < keyCount; ++i)
		{
			if (m_CurrentClip.Keys[i].Tick > m_TickCount)
			{
				keyB = m_CurrentClip.Keys[i];

				if (i > 0)
					keyA = m_CurrentClip.Keys[i - 1];
				else
					keyA = m_CurrentClip.Keys[keyCount - 1];

				break;
			}
		}

		//Interpolate between keys
		auto keyInterval = keyB.Tick - keyA.Tick;
		auto intervalOffset = m_TickCount - keyA.Tick;
		auto interpolation = intervalOffset / keyInterval;

		m_Transforms.clear();

		auto boneCount = keyA.BoneTransforms.size();
		
		for (UINT boneId = 0; boneId < boneCount; ++boneId)
		{
			auto transformA = XMLoadFloat4x4(&keyA.BoneTransforms[boneId]);
			auto transformB = XMLoadFloat4x4(&keyB.BoneTransforms[boneId]);

		 DirectX::XMVECTOR transA, transB, transRes, scaleA, scaleB, scaleRes, rotA, rotB, rotRes;
			XMMatrixDecompose(&scaleA, &rotA, &transA, transformA);
			XMMatrixDecompose(&scaleB, &rotB, &transB, transformB);

			//Interpolate
			transRes = DirectX::XMVectorLerp(transA, transB, interpolation);
			scaleRes = DirectX::XMVectorLerp(scaleA, scaleB, interpolation);
			rotRes = DirectX::XMQuaternionSlerp(rotA, rotB, interpolation);

			//Resulting Matrix
			auto finalMatrix = DirectX::XMMatrixScalingFromVector(scaleRes) * DirectX::XMMatrixRotationQuaternion(rotRes) * DirectX::XMMatrixTranslationFromVector(transRes);
		 DirectX::XMFLOAT4X4 boneTransform;
			XMStoreFloat4x4(&boneTransform, finalMatrix);

			m_Transforms.push_back(boneTransform);
		}
	}
}
