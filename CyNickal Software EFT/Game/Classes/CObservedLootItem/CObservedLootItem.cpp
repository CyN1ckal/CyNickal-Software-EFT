/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CObservedLootItem.h"
#include "Game/Offsets/Offsets.h"
#include "Database/Database.h"
#include "Game/Quest Manager/Quest Manager.h"

CObservedLootItem::CObservedLootItem(uintptr_t EntityAddress) : CBaseLootItem(EntityAddress)
{
	//std::println("[CObservedLootItem] Constructed with {0:X}", m_EntityAddress);
}

void CObservedLootItem::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseLootItem::PrepareRead_1(vmsh);

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CLootItem::pBSGId, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_BSGIdAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CLootItem::pItem, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ItemAddress), nullptr);
}

void CObservedLootItem::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (!m_ItemAddress)
		SetInvalid();

	CBaseLootItem::PrepareRead_2(vmsh);

	VMMDLL_Scatter_PrepareEx(vmsh, m_BSGIdAddress + 0x14, sizeof(m_BSGId), reinterpret_cast<BYTE*>(m_BSGId.data()), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_ItemAddress + Offsets::CItem::pTemplate, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ItemTemplateAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_ItemAddress + Offsets::CItem::StackCount, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_StackCount), nullptr);
}

void CObservedLootItem::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseLootItem::PrepareRead_3(vmsh);

	m_pItemTemplate.emplace(m_ItemTemplateAddress);
	m_pItemTemplate->PrepareRead_1(vmsh);
}

void CObservedLootItem::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseLootItem::PrepareRead_4(vmsh);
	m_pItemTemplate->PrepareRead_2(vmsh);
}

void CObservedLootItem::Finalize()
{
	CBaseLootItem::Finalize();

	m_pItemTemplate->Finalize();
	std::string TarkovIDStr = std::string(m_BSGId.begin(), m_BSGId.end());
	m_ItemPrice = TarkovItemData::GetPriceOfItem(TarkovIDStr);

	if (m_pItemTemplate->IsQuestItem()) {
		m_Name = TarkovQuestItemData::GetQuestItemName(TarkovIDStr);
	}
	else {
		m_Name = TarkovItemData::GetShortNameOfItem(TarkovIDStr);
	}

	if (m_Name.empty())
		std::println("[CObservedLootItem] Failed to get item data for Tarkov ID: {}; {}", TarkovIDStr, m_pItemTemplate->m_sName);

	m_bIsActiveQuestItem = QuestManager::IsItemAssociatedWithAnyActiveQuest(TarkovIDStr);
}

const uint32_t CObservedLootItem::GetSizeInSlots() const
{
	if (m_pItemTemplate)
		return m_pItemTemplate->GetSizeInSlots();

	return 0;
}

const float CObservedLootItem::GetPricePerSlot() const
{
	if (m_pItemTemplate)
	{
		auto Size = m_pItemTemplate->GetSizeInSlots();
		if (Size > 0)
			return static_cast<float>(m_ItemPrice) / static_cast<float>(Size);
	}

	return 0.0f;
}

void CObservedLootItem::QuickRead(VMMDLL_SCATTER_HANDLE vmsh) {
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CLootItem::pItem, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ItemAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CObservedLootItem::QuickFinalize() {
	if(IsInvalid())
		return;	

	if (m_BytesRead != sizeof(uintptr_t)) {
		SetInvalid();
	}

	if (!m_ItemAddress) {
		std::println("[CObservedLootItem] {0:s} was picked up", GetName());
		SetInvalid();
	}
}