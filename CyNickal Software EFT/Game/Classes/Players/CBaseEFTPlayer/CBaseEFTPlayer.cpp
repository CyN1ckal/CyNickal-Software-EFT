#include "pch.h"
#include "CBaseEFTPlayer.h"
#include "Game/Offsets/Offsets.h"
#include "Game/Classes/CUnityTransform/CUnityTransform.h"

void CBaseEFTPlayer::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh, EPlayerType playerType)
{
	if (IsInvalid()) return;

	if (playerType == EPlayerType::eObservedPlayer)
	{
		VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CObservedPlayer::pPlayerBody, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_PlayerBodyAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
		VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CObservedPlayer::pAiData, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_AIDataAddress), nullptr);
	}
	else if (playerType == EPlayerType::eMainPlayer)
	{
		VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CPlayer::pPlayerBody, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_PlayerBodyAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
		VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CPlayer::pAiData, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_AIDataAddress), nullptr);
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

	if (m_AIDataAddress)
	{
		VMMDLL_Scatter_PrepareEx(vmsh, m_AIDataAddress + Offsets::CAIData::bIsAi, sizeof(std::byte), reinterpret_cast<BYTE*>(&m_AiByte), nullptr);
		VMMDLL_Scatter_PrepareEx(vmsh, m_AIDataAddress + Offsets::CAIData::pBotOwner, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_BotOwnerAddress), nullptr);
	}
}

void CBaseEFTPlayer::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_SkeletonRootAddress + Offsets::CSkeleton::pSkeletonValues, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_SkeletonValuesAddress), reinterpret_cast<DWORD*>(&m_BytesRead));

	if (m_BotOwnerAddress)
		VMMDLL_Scatter_PrepareEx(vmsh, m_BotOwnerAddress + Offsets::CBotOwner::pSpawnProfileData, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_SpawnProfileDataAddress), nullptr);
}

void CBaseEFTPlayer::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_SkeletonValuesAddress + Offsets::CSkeletonValues::pBoneArray, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_BoneArrayAddress), reinterpret_cast<DWORD*>(&m_BytesRead));

	if (m_SpawnProfileDataAddress)
		VMMDLL_Scatter_PrepareEx(vmsh, m_SpawnProfileDataAddress + Offsets::CSpawnProfileData::SpawnType, sizeof(ESpawnType), reinterpret_cast<BYTE*>(&m_SpawnType), nullptr);
}

void CBaseEFTPlayer::PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_BoneArrayAddress + Offsets::CBoneArray::ArrayStart + (0x8 * std::to_underlying(EBoneIndex::Root)), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_HumanRootPtrAddr), nullptr);
}

void CBaseEFTPlayer::PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_HumanRootPtrAddr + 0x10, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_HumanRootAddr), nullptr);
}

void CBaseEFTPlayer::PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_Transforms.clear();

	/* Bone at index [0] is root */
	m_Transforms.emplace_back(CUnityTransform(m_HumanRootAddr));
	//m_Transforms.emplace_back(CUnityTransform(m_HumanHeadPtrAddr));

	for (auto& Transform : m_Transforms)
		Transform.PrepareRead_1(vmsh);
}

void CBaseEFTPlayer::PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	for (auto& Transform : m_Transforms)
		Transform.PrepareRead_2(vmsh);
}

void CBaseEFTPlayer::PrepareRead_9(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	for (auto& Transform : m_Transforms)
		Transform.PrepareRead_3(vmsh);
}

void CBaseEFTPlayer::PrepareRead_10(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	for (auto& Transform : m_Transforms)
		Transform.PrepareRead_4(vmsh);
}

void CBaseEFTPlayer::Finalize()
{
	if (IsInvalid())
		return;

	m_RootPosition = m_Transforms[0].GetPosition();
}

void CBaseEFTPlayer::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	for (auto& Transform : m_Transforms)
		Transform.QuickRead(vmsh);
}

void CBaseEFTPlayer::QuickFinalize()
{
	if (IsInvalid())
		return;

	m_RootPosition = m_Transforms[0].GetPosition();
}

const bool CBaseEFTPlayer::IsAi() const
{
	return m_AiByte != std::byte{ 0 };
}

const bool CBaseEFTPlayer::IsPMC() const
{
	return (m_Side == EPlayerSide::USEC || m_Side == EPlayerSide::BEAR);
}

const bool CBaseEFTPlayer::IsPlayerScav() const
{
	return IsAi() == false && m_Side == EPlayerSide::SCAV;
}

const std::string PMCLabel = "PMC";
const std::string PlayerScavLabel = "PScav";
const std::string ScavLabel = "Scav";
const std::string& CBaseEFTPlayer::GetBaseName() const
{
	if (IsBoss())
		return GetBossName();

	if (IsPlayerScav())
		return PlayerScavLabel;

	if (IsAi())
		return ScavLabel;

	return PMCLabel;
}

#include "BossNameMap.h"
const std::string BossLabel = "Boss";
const std::string& CBaseEFTPlayer::GetBossName() const
{
	return BossNameMap.contains(m_SpawnType) ? BossNameMap.at(m_SpawnType) : BossLabel;
}

#include "GUI/Color Picker/Color Picker.h"
const ImColor CBaseEFTPlayer::GetSideColor() const
{
	if (IsBoss())
		return ColorPicker::m_BossColor;

	if (IsPlayerScav())
		return ColorPicker::m_PlayerScavColor;

	if (IsAi())
		return ColorPicker::m_ScavColor;

	return ColorPicker::m_PMCColor;
}

const bool CBaseEFTPlayer::IsBoss() const
{
	switch (m_SpawnType)
	{
	case ESpawnType::BigPipe:
	case ESpawnType::BirdEye:
	case ESpawnType::Knight:
	case ESpawnType::Killa:
	case ESpawnType::Kaban:
	case ESpawnType::Partisan:
	case ESpawnType::Sanitar:
	case ESpawnType::Gluhar:
	case ESpawnType::Kolontay:
	case ESpawnType::Tagilla:
	case ESpawnType::Shturman:
	case ESpawnType::Reshala:
		return true;
	default:
		return false;
	}
}

const bool CBaseEFTPlayer::IsInvalid() const
{
	return CBaseEntity::IsInvalid();
}