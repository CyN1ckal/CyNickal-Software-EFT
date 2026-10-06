/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CBaseLootItem.h"
#include "Game/Offsets/Offsets.h"

CBaseLootItem::CBaseLootItem(uintptr_t EntityAddress) : CBaseEntity(EntityAddress)
{
}

void CBaseLootItem::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + 0x10, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_MonoBehaviorAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CBaseLootItem::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (!m_MonoBehaviorAddress)
		SetInvalid();

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_MonoBehaviorAddress + Offsets::CMonoBehavior::pGameObject, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_GameObjectAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CBaseLootItem::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (!m_GameObjectAddress)
		SetInvalid();

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_GameObjectAddress + Offsets::CGameObject::pComponents, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ComponentsAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CBaseLootItem::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_ComponentsAddress + Offsets::CComponents::pTransform, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_TransformAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CBaseLootItem::PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid()) return;

	m_pTransform.emplace(m_TransformAddress);
	m_pTransform->PrepareRead_1(vmsh);
}

void CBaseLootItem::PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_pTransform->PrepareRead_2(vmsh);
}

void CBaseLootItem::PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_pTransform->PrepareRead_3(vmsh);
}

void CBaseLootItem::PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_pTransform->PrepareRead_4(vmsh);
}

void CBaseLootItem::Finalize()
{
	if (!m_pTransform || m_pTransform->IsInvalid())
		SetInvalid();

	if (IsInvalid()) return;

	m_Position = m_pTransform->GetPosition();
}

void CBaseLootItem::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_pTransform->QuickRead(vmsh);
}

void CBaseLootItem::QuickFinalize()
{
	if (IsInvalid()) return;

	m_pTransform->QuickFinalize();
}