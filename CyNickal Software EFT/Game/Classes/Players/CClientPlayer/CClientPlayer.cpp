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
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CPlayer::pProceduralWeaponAnimation, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ProceduralWeaponAnimationAddress), nullptr);
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
	VMMDLL_Scatter_PrepareEx(vmsh, m_ProceduralWeaponAnimationAddress + Offsets::CProceduralWeaponAnimation::pOptics, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_OpticsAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_ProceduralWeaponAnimationAddress + Offsets::CProceduralWeaponAnimation::bAiming, sizeof(std::byte), reinterpret_cast<BYTE*>(&m_AimingByte), nullptr);
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

void CClientPlayer::PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_6(vmsh);

	if (IsInvalid())
		return;

	m_pHands->PrepareRead_5(vmsh);
}

void CClientPlayer::PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_7(vmsh);

	if (IsInvalid())
		return;

	m_pHands->PrepareRead_6(vmsh);
}

void CClientPlayer::PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_8(vmsh);

	if (IsInvalid())
		return;

	m_pHands->PrepareRead_7(vmsh);
}

void CClientPlayer::PrepareRead_9(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_9(vmsh);

	if (IsInvalid())
		return;

	m_pHands->PrepareRead_8(vmsh);
}

void CClientPlayer::PrepareRead_10(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_10(vmsh);

	if (IsInvalid())
		return;

	m_pHands->PrepareRead_9(vmsh);
}

void CClientPlayer::PrepareRead_11(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid())
		return;

	m_pHands->PrepareRead_10(vmsh);
}

/* Empty methods to keep interface same as CObservedPlayer */
void CClientPlayer::PrepareRead_12(VMMDLL_SCATTER_HANDLE vmsh)
{
}

void CClientPlayer::PrepareRead_13(VMMDLL_SCATTER_HANDLE vmsh)
{
}

void CClientPlayer::PrepareRead_14(VMMDLL_SCATTER_HANDLE vmsh)
{
}

void CClientPlayer::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::QuickRead(vmsh, EPlayerType::eMainPlayer);

	if (IsInvalid())
		return;

	if(m_pHands)
		m_pHands->QuickRead(vmsh, EPlayerType::eMainPlayer);

	VMMDLL_Scatter_PrepareEx(vmsh, m_MovementContextAddress + Offsets::CMovementContext::Rotation, sizeof(float), reinterpret_cast<BYTE*>(&m_Yaw), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CPlayer::pHandsController, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_HandsControllerAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_ProceduralWeaponAnimationAddress + Offsets::CProceduralWeaponAnimation::bAiming, sizeof(std::byte), reinterpret_cast<BYTE*>(&m_AimingByte), nullptr);
}

void CClientPlayer::Finalize()
{
	CBaseEFTPlayer::Finalize();

	if (IsInvalid())
		return;
}

void CClientPlayer::QuickFinalize()
{
	CBaseEFTPlayer::QuickFinalize();

	if (IsInvalid())
		return;

	if (m_HandsControllerAddress == m_PreviousHandsControllerAddress) return;

	m_PreviousHandsControllerAddress = m_HandsControllerAddress;
	m_pHands = std::make_unique<CHeldItem>(m_HandsControllerAddress);
	m_pHands->CompleteUpdate(EPlayerType::eMainPlayer);
}