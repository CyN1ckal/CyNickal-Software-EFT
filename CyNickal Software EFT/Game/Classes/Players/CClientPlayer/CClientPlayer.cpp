#include "pch.h"

#include "CClientPlayer.h"

#include "Game/Offsets/Offsets.h"

void CClientPlayer::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_1(vmsh, EPlayerType::eMainPlayer);

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CPlayer::pMovementContext, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_MovementContextAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CPlayer::pProfile, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ProfileAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CPlayer::pHandsController, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_HandsControllerAddress), nullptr);
}

void CClientPlayer::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_2(vmsh);

	if (!m_HandsControllerAddress)
		SetInvalid();

	if (IsInvalid())
		return;

	m_pHands = std::make_unique<CHeldItem>(m_HandsControllerAddress);
	m_PreviousHandsControllerAddress = m_HandsControllerAddress;
	m_pHands->PrepareRead_1(vmsh, EPlayerType::eMainPlayer);

	VMMDLL_Scatter_PrepareEx(vmsh, m_ProfileAddress + Offsets::CProfile::pProfileInfo, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ProfileInfoAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_MovementContextAddress + Offsets::CMovementContext::Rotation, sizeof(float), reinterpret_cast<BYTE*>(&m_Yaw), nullptr);
}

void CClientPlayer::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_3(vmsh);

	if (IsInvalid())
		return;

	m_pHands->PrepareRead_2(vmsh);

	VMMDLL_Scatter_PrepareEx(vmsh, m_ProfileInfoAddress + Offsets::CProfileInfo::Side, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_Side), nullptr);
}

void CClientPlayer::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_4(vmsh);

	if (IsInvalid())
		return;

	m_pHands->PrepareRead_3(vmsh);
}

void CClientPlayer::PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_5(vmsh);

	if (IsInvalid())
		return;

	m_pHands->PrepareRead_4(vmsh);
}

void CClientPlayer::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::QuickRead(vmsh, EPlayerType::eMainPlayer);

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_MovementContextAddress + Offsets::CMovementContext::Rotation, sizeof(float), reinterpret_cast<BYTE*>(&m_Yaw), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CPlayer::pHandsController, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_HandsControllerAddress), nullptr);
}

void CClientPlayer::Finalize()
{
	CBaseEFTPlayer::Finalize();

	if (IsInvalid())
		return;

	m_pHands->Finalize();
}

void CClientPlayer::QuickFinalize()
{
	CBaseEFTPlayer::QuickFinalize();

	if (IsInvalid())
		return;

	if (m_HandsControllerAddress = m_PreviousHandsControllerAddress) return;

	m_PreviousHandsControllerAddress = m_HandsControllerAddress;
	m_pHands = std::make_unique<CHeldItem>(m_HandsControllerAddress);
	m_pHands->CompleteUpdate(EPlayerType::eMainPlayer);
}