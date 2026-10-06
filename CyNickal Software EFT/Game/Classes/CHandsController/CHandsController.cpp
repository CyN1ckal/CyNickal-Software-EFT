/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CHandsController.h"
#include "Game/Offsets/Offsets.h"
#include "DMA/CDMAConnection/CDMAConnection.h"
#include "Game/EFT.h"

CHandsController::CHandsController(uintptr_t EntityAddress) : CBaseEntity(EntityAddress) {}

void CHandsController::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh, EPlayerType PlayerType)
{
	if (PlayerType == EPlayerType::eMainPlayer)
		VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CHandsController::pItem, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_HeldItemAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	else if (PlayerType == EPlayerType::eObservedPlayer)
		VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CObservedPlayerHands::pItem, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_HeldItemAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CHandsController::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (!m_HeldItemAddress)
		SetInvalid();

	if (IsInvalid()) return;

	m_pHeldItem.emplace(m_HeldItemAddress);
	m_PreviousHeldItemAddress = m_HeldItemAddress;
	m_pHeldItem->PrepareRead_1(vmsh);

	VMMDLL_Scatter_PrepareEx(vmsh, m_HeldItemAddress + Offsets::CItem::pMagslot, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_MagazineAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CHandsController::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid()) return;

	if (m_MagazineAddress)
	{
		m_pMagazine.emplace(m_MagazineAddress);
		m_pMagazine->PrepareRead_1(vmsh);
	}

	m_pHeldItem->PrepareRead_2(vmsh);
}

void CHandsController::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	if (m_pMagazine)
		m_pMagazine->PrepareRead_2(vmsh);

	m_pHeldItem->PrepareRead_3(vmsh);
}

void CHandsController::PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	if (m_pMagazine)
		m_pMagazine->PrepareRead_3(vmsh);
}

void CHandsController::PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	if (m_pMagazine)
		m_pMagazine->PrepareRead_4(vmsh);
}

void CHandsController::PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	if (m_pMagazine)
		m_pMagazine->PrepareRead_5(vmsh);
}

void CHandsController::PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	if (m_pMagazine)
		m_pMagazine->PrepareRead_6(vmsh);
}

void CHandsController::PrepareRead_9(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	if (m_pMagazine)
		m_pMagazine->PrepareRead_7(vmsh);
}

void CHandsController::PrepareRead_10(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	if (m_pMagazine)
		m_pMagazine->PrepareRead_8(vmsh);
}

void CHandsController::QuickRead(VMMDLL_SCATTER_HANDLE vmsh, EPlayerType PlayerType)
{
	if (IsInvalid()) return;

	if (PlayerType == EPlayerType::eMainPlayer)
		VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CHandsController::pItem, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_HeldItemAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	else if (PlayerType == EPlayerType::eObservedPlayer)
		VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CObservedPlayerHands::pItem, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_HeldItemAddress), reinterpret_cast<DWORD*>(&m_BytesRead));

	if (m_pMagazine)
		m_pMagazine->QuickRead(vmsh);
}

void CHandsController::Finalize()
{
	if (!m_pHeldItem || m_pHeldItem->IsInvalid())
		SetInvalid();

	if (m_pMagazine && m_pMagazine->IsInvalid())
		m_pMagazine = std::nullopt;

	if (IsInvalid()) return;

	if (m_pMagazine)
		m_pMagazine->Finalize();

	m_pHeldItem->Finalize();
}

void CHandsController::QuickFinalize()
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid()) return;

	if (m_HeldItemAddress == m_PreviousHeldItemAddress)
		return;

	m_PreviousHeldItemAddress = m_HeldItemAddress;

	if (m_HeldItemAddress == 0)
	{
		m_pHeldItem = std::nullopt;
		return;
	}

	m_pHeldItem.emplace(m_HeldItemAddress, EAllocationType::BLOCKING);
}

void CHandsController::CompleteUpdate(EPlayerType PlayerType)
{
	auto Conn = CDMAConnection::GetInstance();

	auto PID = EFT::GetProcess().GetPID();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), PID, VMMDLL_FLAG_NOCACHE);
	PrepareRead_1(vmsh, PlayerType);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_2(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_3(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_4(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_5(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_6(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_7(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_8(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_9(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_10(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	VMMDLL_Scatter_CloseHandle(vmsh);

	Finalize();
}