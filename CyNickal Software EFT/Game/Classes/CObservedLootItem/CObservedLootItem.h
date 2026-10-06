/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "Game/Classes/CBaseLootItem/CBaseLootItem.h"
#include "Game/Classes/CItemTemplate/CItemTemplate.h"

class CObservedLootItem : public CBaseLootItem
{
public:
	CObservedLootItem(uintptr_t EntityAddress);
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickFinalize();

public:
	int32_t GetItemPrice() const { return m_ItemPrice; }
	const std::string& GetName() const { return m_Name; }
	const uint32_t GetSizeInSlots() const;
	const float GetPricePerSlot() const;
	const uint32_t GetStackCount() const { return m_StackCount; }
	const bool IsQuestItem() const { return m_pItemTemplate && m_pItemTemplate->IsQuestItem(); }
	const bool IsActiveTaskItem() const { return m_bIsActiveQuestItem; }

private:
	std::string m_Name{""};
	std::optional<CItemTemplate> m_pItemTemplate{ std::nullopt };
	std::array<wchar_t, 24> m_BSGId{};
	uintptr_t m_BSGIdAddress{ 0 };
	uintptr_t m_ItemAddress{ 0 };
	uintptr_t m_ItemTemplateAddress{ 0 };
	int32_t m_ItemPrice{ -1 };
	uint32_t m_StackCount{ 0 };
	bool m_bIsActiveQuestItem{ false };
};