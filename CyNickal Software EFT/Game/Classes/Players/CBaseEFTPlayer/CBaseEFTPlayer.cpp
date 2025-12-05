#include "pch.h"
#include "CBaseEFTPlayer.h"
#include "Game/Offsets/Offsets.h"
#include "Game/Classes/CPlayer.h"

void CBaseEFTPlayer::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh, EPlayerType playerType)
{
	if (IsInvalid()) return;

	if (playerType == EPlayerType::eObservedPlayer)
	{
		uintptr_t pPlayerBody = m_EntityAddress + Offsets::CObservedPlayer::pPlayerBody;
		VMMDLL_Scatter_PrepareEx(vmsh, pPlayerBody, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_PlayerBodyAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	}
	else if (playerType == EPlayerType::eMainPlayer)
	{
		uintptr_t pPlayerBody = m_EntityAddress + Offsets::CPlayer::pPlayerBody;
		VMMDLL_Scatter_PrepareEx(vmsh, pPlayerBody, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_PlayerBodyAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	}
}

void CBaseEFTPlayer::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (!m_PlayerBodyAddress)
		SetInvalid();

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_PlayerBodyAddress + Offsets::CPlayerBody::pSkeleton, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_SkeletonRootAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CBaseEFTPlayer::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_SkeletonRootAddress + Offsets::CSkeleton::pValues, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ValuesArrayAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CBaseEFTPlayer::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_ValuesArrayAddress + Offsets::CValues::pArr1, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_Arr1Address), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CBaseEFTPlayer::PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_Arr1Address + offsetof(CArr1, pBoneTransforms), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_BoneTransformsAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CBaseEFTPlayer::PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_BoneTransformsAddress + offsetof(CBoneTransforms, pBaseTransform), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_BasePositionTransformAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CBaseEFTPlayer::PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_BasePositionTransformAddress + offsetof(CUnityTransform, pTransformHierarchy), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_TransformHierarchyAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CBaseEFTPlayer::PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_TransformHierarchyAddress + Offsets::CTransformHierarchy::Position, sizeof(Vector3), reinterpret_cast<BYTE*>(&m_RootPosition), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CBaseEFTPlayer::Finalize()
{
	if (m_BytesRead != sizeof(Vector3))
		SetInvalid();
}

void CBaseEFTPlayer::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_TransformHierarchyAddress + Offsets::CTransformHierarchy::Position, sizeof(Vector3), reinterpret_cast<BYTE*>(&m_RootPosition), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CBaseEFTPlayer::QuickFinalize()
{
	if (m_BytesRead != sizeof(Vector3))
		SetInvalid();
}

bool CBaseEFTPlayer::IsAi() const
{
	return m_AiByte != std::byte{ 0 };
}

bool CBaseEFTPlayer::IsPMC() const
{
	return (m_Side == EPlayerSide::USEC || m_Side == EPlayerSide::BEAR);
}

bool CBaseEFTPlayer::IsPlayerScav() const
{
	return IsAi() == false && m_Side == EPlayerSide::SCAV;
}

const std::string PMCLabel = "PMC";
const std::string PlayerScavLabel = "PScav";
const std::string ScavLabel = "Scav";
const std::string& CBaseEFTPlayer::GetBaseName() const
{
	if (IsPlayerScav())
		return PlayerScavLabel;

	if (IsAi())
		return ScavLabel;

	return PMCLabel;
}

#include "GUI/Color Picker/Color Picker.h"
const ImColor CBaseEFTPlayer::GetSideColor() const
{
	if (IsPlayerScav())
		return ColorPicker::m_PlayerScavColor;

	if (IsAi())
		return ColorPicker::m_ScavColor;

	return ColorPicker::m_PMCColor;
}
