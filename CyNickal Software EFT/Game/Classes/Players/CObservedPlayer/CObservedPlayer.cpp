#include "pch.h"
#include "CObservedPlayer.h"
#include "Game/Offsets/Offsets.h"

void CObservedPlayer::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_1(vmsh, EPlayerType::eObservedPlayer);

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CObservedPlayer::IsAi, sizeof(std::byte), reinterpret_cast<BYTE*>(&m_AiByte), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CObservedPlayer::pPlayerController, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_PlayerControllerAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CObservedPlayer::PlayerSide, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_Side), nullptr);
}

void CObservedPlayer::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_2(vmsh);

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_PlayerControllerAddress + Offsets::CObservedPlayerController::pMovementController, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_MovementControllerAddress), nullptr);
}

void CObservedPlayer::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_3(vmsh);

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_MovementControllerAddress + Offsets::CMovementController::pObservedPlayerState, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ObservedMovementStateAddress), nullptr);
}

void CObservedPlayer::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_4(vmsh);

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_ObservedMovementStateAddress + Offsets::CObservedMovementState::Rotation, sizeof(float), reinterpret_cast<BYTE*>(&m_Yaw), nullptr);
}

void CObservedPlayer::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::QuickRead(vmsh);

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_ObservedMovementStateAddress + Offsets::CObservedMovementState::Rotation, sizeof(float), reinterpret_cast<BYTE*>(&m_Yaw), nullptr);
}