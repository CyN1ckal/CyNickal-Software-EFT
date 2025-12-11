#include "pch.h"
#include "CBaseEFTPlayer.h"
#include "Game/Offsets/Offsets.h"
#include "Game/GOM/GOM.h"

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

	m_pSkeleton = std::make_unique<CPlayerSkeleton>(m_SkeletonRootAddress);
	m_pSkeleton->PrepareRead_1(vmsh);

	if (m_BotOwnerAddress)
		VMMDLL_Scatter_PrepareEx(vmsh, m_BotOwnerAddress + Offsets::CBotOwner::pSpawnProfileData, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_SpawnProfileDataAddress), nullptr);
}

void CBaseEFTPlayer::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_pSkeleton->PrepareRead_2(vmsh);

	if (m_SpawnProfileDataAddress)
		VMMDLL_Scatter_PrepareEx(vmsh, m_SpawnProfileDataAddress + Offsets::CSpawnProfileData::SpawnType, sizeof(ESpawnType), reinterpret_cast<BYTE*>(&m_SpawnType), nullptr);
}

void CBaseEFTPlayer::PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_pSkeleton->PrepareRead_3(vmsh);
}

void CBaseEFTPlayer::PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_pSkeleton->PrepareRead_4(vmsh);
}

void CBaseEFTPlayer::PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_pSkeleton->PrepareRead_5(vmsh);
}

void CBaseEFTPlayer::PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_pSkeleton->PrepareRead_6(vmsh);
}

void CBaseEFTPlayer::PrepareRead_9(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_pSkeleton->PrepareRead_7(vmsh);
}

void CBaseEFTPlayer::PrepareRead_10(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_pSkeleton->PrepareRead_8(vmsh);
}

void CBaseEFTPlayer::Finalize()
{
	if (IsInvalid())
		return;

	if (m_EntityAddress == GOM::GetMainPlayerAddress())
		SetLocalPlayer();

	m_pSkeleton->Finalize();
}

void CBaseEFTPlayer::QuickRead(VMMDLL_SCATTER_HANDLE vmsh, EPlayerType playerType)
{
	if (IsInvalid()) return;

	m_pSkeleton->QuickRead(vmsh);
}

void CBaseEFTPlayer::QuickFinalize()
{
	if (IsInvalid())
		return;

	if (m_pSkeleton)
		m_pSkeleton->QuickFinalize();
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

static Vector3 invalidPosition{ 0.0f,0.0f,0.0f };
const Vector3& CBaseEFTPlayer::GetBonePosition(EBoneIndex boneIndex) const
{
	if (m_pSkeleton == nullptr)
		return invalidPosition;

	return m_pSkeleton->GetBonePosition(boneIndex);
}

const bool CBaseEFTPlayer::IsLocalPlayer() const
{
	return m_Flags & 0x2;
}

void CBaseEFTPlayer::SetLocalPlayer()
{
	m_Flags |= 0x2;
}