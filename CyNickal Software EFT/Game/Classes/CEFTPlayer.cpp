#include "pch.h"
#include "CEFTPlayer.h"
#include "CPlayer.h"
#include "Game/Offsets/Offsets.h"

void CEFTPlayer::SetInvalid()
{
	m_Flags |= 0x1;
}

bool CEFTPlayer::IsInvalid() const
{
	return m_Flags & 0x1;
}

void CEFTPlayer::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CPlayer::pPlayerBody, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_PlayerBodyAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CPlayer::pMovementContext, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_MovementContextAddress), nullptr);
}

void CEFTPlayer::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t) || !m_PlayerBodyAddress || !m_MovementContextAddress)
	{
		SetInvalid();
		std::println("[CEFTPlayer] Invalid level 1 read");
	}

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_PlayerBodyAddress + Offsets::CPlayerBody::pSkeleton, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_SkeletonRootAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	VMMDLL_Scatter_PrepareEx(vmsh, m_MovementContextAddress + Offsets::CMovementContext::Rotation, sizeof(float), reinterpret_cast<BYTE*>(&m_Yaw), nullptr);
}

void CEFTPlayer::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t) || !m_SkeletonRootAddress)
	{
		SetInvalid();
		std::println("[CEFTPlayer] Invalid Skeleton Root Address.");
	}

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_SkeletonRootAddress + offsetof(CSkeleton, pValues), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ValuesArrayAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CEFTPlayer::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t) || !m_ValuesArrayAddress)
	{
		SetInvalid();
		std::println("[CEFTPlayer] Invalid Values Array Address.");
	}

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_ValuesArrayAddress + offsetof(CValues, pArr1), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_Arr1Address), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CEFTPlayer::PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t) || !m_Arr1Address)
	{
		SetInvalid();
		std::println("[CEFTPlayer] Invalid Arr1 Address.");
	}

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_Arr1Address + offsetof(CArr1, pBoneTransforms), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_BoneTransformsAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CEFTPlayer::PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t) || !m_BoneTransformsAddress)
	{
		SetInvalid();
		std::println("[CEFTPlayer] Invalid Bone Transforms Address.");
	}

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_BoneTransformsAddress + offsetof(CBoneTransforms, pBaseTransform), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_BasePositionTransformAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CEFTPlayer::PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t) || !m_BasePositionTransformAddress)
	{
		SetInvalid();
		std::println("[CEFTPlayer] Invalid Base Position Transform Address.");
	}

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_BasePositionTransformAddress + offsetof(CUnityTransform, pTransformHierarchy), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_TransformHierarchyAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CEFTPlayer::PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t) || !m_TransformHierarchyAddress)
	{
		SetInvalid();
		std::println("[CEFTPlayer] Invalid Transform Hierarchy Address.");
	}

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_TransformHierarchyAddress + offsetof(CTransformHierarchy, Position), sizeof(Vector3), reinterpret_cast<BYTE*>(&m_BasePosition), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CEFTPlayer::Finalize()
{
	if (m_BytesRead != sizeof(Vector3))
		SetInvalid();
}

void CEFTPlayer::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_MovementContextAddress + Offsets::CMovementContext::Rotation, sizeof(float), reinterpret_cast<BYTE*>(&m_Yaw), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_TransformHierarchyAddress + offsetof(CTransformHierarchy, Position), sizeof(Vector3), reinterpret_cast<BYTE*>(&m_BasePosition), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CEFTPlayer::QuickFinalize()
{
	if (m_BytesRead != sizeof(Vector3))
		SetInvalid();
}