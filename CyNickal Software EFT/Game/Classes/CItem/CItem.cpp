/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CItem.h"
#include "Game/Offsets/Offsets.h"
#include "DMA/CDMAConnection/CDMAConnection.h"
#include "Game/EFT.h"
#include "Database/Database.h"

CItem::CItem(uintptr_t EntityAddress, EAllocationType AllocType) : CBaseEntity(EntityAddress)
{
	//std::println("[CItem] constructed {0:X}", m_EntityAddress);

	if (AllocType == EAllocationType::BLOCKING) {
		CompleteUpdate();
	}
}

void CItem::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CItem::pTemplate, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ItemTemplateAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CItem::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (!m_ItemTemplateAddress)
		SetInvalid();

	if (IsInvalid()) return;

	m_pItemTemplate.emplace(m_ItemTemplateAddress);
	m_pItemTemplate->PrepareRead_1(vmsh);
}

void CItem::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_pItemTemplate->PrepareRead_2(vmsh);
}

void CItem::CompleteUpdate()
{
	auto Conn = CDMAConnection::GetInstance();
	auto PID = EFT::GetProcess().GetPID();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_1(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_2(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_3(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	Finalize();
}

template <typename T>
uint32_t JOAAT(T String) {
	size_t i = 0;
	uint32_t hash = 0;
	while (i != String.size()) {
		hash += String[i++];
		hash += hash << 10;
		hash ^= hash >> 6;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
	return hash;
}

void CItem::Finalize()
{
	if (!m_pItemTemplate || m_pItemTemplate->IsInvalid())
		SetInvalid();

	if (IsInvalid()) return;

	m_pItemTemplate->Finalize();

	m_ItemName = TarkovItemData::GetShortNameOfItem(m_pItemTemplate->m_sTarkovID);
}

const std::string& CItem::GetItemName() const
{
	return m_ItemName;
}
