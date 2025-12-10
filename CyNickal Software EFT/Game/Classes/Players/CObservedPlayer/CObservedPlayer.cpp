#include "pch.h"
#include "CObservedPlayer.h"
#include "Game/Offsets/Offsets.h"

void CObservedPlayer::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_1(vmsh, EPlayerType::eObservedPlayer);

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CObservedPlayer::pPlayerController, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_PlayerControllerAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CObservedPlayer::PlayerSide, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_Side), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CObservedPlayer::pVoice, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_VoiceAddress), nullptr);
}

void CObservedPlayer::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_2(vmsh);

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_PlayerControllerAddress + Offsets::CObservedPlayerController::pMovementController, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_MovementControllerAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_PlayerControllerAddress + Offsets::CObservedPlayerController::pHealthController, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_HealthControllerAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_VoiceAddress + 0x14, sizeof(m_wVoice), reinterpret_cast<BYTE*>(&m_wVoice), nullptr);
}

void CObservedPlayer::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_3(vmsh);

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_MovementControllerAddress + Offsets::CMovementController::pObservedPlayerState, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ObservedMovementStateAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_HealthControllerAddress + Offsets::CHealthController::HealthStatus, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_TagStatus), nullptr);
}

void CObservedPlayer::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_4(vmsh);

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_ObservedMovementStateAddress + Offsets::CObservedMovementState::Rotation, sizeof(float), reinterpret_cast<BYTE*>(&m_Yaw), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_ObservedMovementStateAddress + Offsets::CObservedMovementState::pObservedPlayerHands, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ObservedHandsControllerAddress), nullptr);
}

void CObservedPlayer::PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_5(vmsh);

	if (IsInvalid())
		return;

	m_pHands = std::make_unique<CHeldItem>(m_ObservedHandsControllerAddress);
	m_PreviousHandsControllerAddress = m_ObservedHandsControllerAddress;
	m_pHands->PrepareRead_1(vmsh, EPlayerType::eObservedPlayer);
}

void CObservedPlayer::PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_6(vmsh);

	if (IsInvalid())
		return;

	m_pHands->PrepareRead_2(vmsh);
}

void CObservedPlayer::PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_7(vmsh);

	if (IsInvalid())
		return;

	m_pHands->PrepareRead_3(vmsh);
}

void CObservedPlayer::PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_8(vmsh);

	if (IsInvalid())
		return;

	m_pHands->PrepareRead_4(vmsh);
}

void CObservedPlayer::Finalize()
{
	CBaseEFTPlayer::Finalize();

	if (IsInvalid())
		return;

	m_pHands->Finalize();

	for (int i = 0; i < 32; i++)
		m_Voice[i] = static_cast<char>(m_wVoice[i]);

	if (m_SpawnType == ESpawnType::UNKNOWN && m_Voice[1] == 'o')
		SetSpawnTypeFromVoice();
}

void CObservedPlayer::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::QuickRead(vmsh, EPlayerType::eObservedPlayer);

	if (IsInvalid())
		return;

	m_pHands->QuickRead(vmsh, EPlayerType::eObservedPlayer);

	VMMDLL_Scatter_PrepareEx(vmsh, m_HealthControllerAddress + Offsets::CHealthController::HealthStatus, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_TagStatus), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_ObservedMovementStateAddress + Offsets::CObservedMovementState::Rotation, sizeof(float), reinterpret_cast<BYTE*>(&m_Yaw), nullptr);
}

void CObservedPlayer::QuickFinalize()
{
	CBaseEFTPlayer::QuickFinalize();

	if (IsInvalid()) return;

	m_pHands->QuickFinalize();
}

const bool CObservedPlayer::IsInCondition(const ETagStatus status) const
{
	std::bitset<32> statusBits(m_TagStatus);

	return statusBits.test(std::to_underlying(status));
}

const bool CObservedPlayer::IsInCriticalHealthStatus() const
{
	return IsInCondition(ETagStatus::Injured) || IsInCondition(ETagStatus::BadlyInjured) || IsInCondition(ETagStatus::Dying);
}

void CObservedPlayer::SetSpawnTypeFromVoice()
{
	if (m_Voice[4] == '_')
	{
		if (m_Voice[5] == 'S')
		{
			m_SpawnType = ESpawnType::Shturman;
			return;
		}
		else if (m_Voice[5] == 'P')
		{
			m_SpawnType = ESpawnType::Partisan;
			return;
		}
		else if (m_Voice[6] == 'a')
		{
			m_SpawnType = ESpawnType::Kaban;
			return;
		}
		else if (m_Voice[6] == 'o')
		{
			m_SpawnType = ESpawnType::Kolontay;
			return;
		}
	}
	else
	{
		if (m_Voice[4] == 'S')
		{
			m_SpawnType = ESpawnType::Sanitar;
			return;
		}
		else if (m_Voice[4] == 'G')
		{
			m_SpawnType = ESpawnType::Gluhar;
			return;
		}
		else if (m_Voice[4] == 'T')
		{
			m_SpawnType = ESpawnType::Tagilla;
			return;
		}
		else if (m_Voice[5] == 'n')
		{
			m_SpawnType = ESpawnType::Knight;
			return;
		}
		else if (m_Voice[5] == 'u')
		{
			m_SpawnType = ESpawnType::Reshala;
			return;
		}
		else if (m_Voice[6] == 'l')
		{
			m_SpawnType = ESpawnType::Killa;
			return;
		}
		else if (m_Voice[6] == 'g')
		{
			m_SpawnType = ESpawnType::BigPipe;
			return;
		}
		else if (m_Voice[6] == 'r')
		{
			m_SpawnType = ESpawnType::BirdEye;
			return;
		}
	}
}