#include "pch.h"

#include "CClientPlayer.h"

#include "Game/Offsets/Offsets.h"

void CClientPlayer::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_1(vmsh, EPlayerType::eMainPlayer);

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CPlayer::pMovementContext, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_MovementContextAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CPlayer::pAiData, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_AiDataAddress), nullptr);
}

void CClientPlayer::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_2(vmsh);

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_MovementContextAddress + Offsets::CMovementContext::Rotation, sizeof(float), reinterpret_cast<BYTE*>(&m_Yaw), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_AiDataAddress + Offsets::CAIData::bIsAi, sizeof(std::byte), reinterpret_cast<BYTE*>(&m_AiByte), nullptr);
}

void CClientPlayer::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::QuickRead(vmsh);

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_MovementContextAddress + Offsets::CMovementContext::Rotation, sizeof(float), reinterpret_cast<BYTE*>(&m_Yaw), nullptr);
}

const bool CClientPlayer::IsAi() const
{
	return m_AiByte != std::byte{ 0 };
}