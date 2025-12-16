#include "pch.h"

#include "CObservedLootItem.h"

#include "Game/Offsets/Offsets.h"

#include "Database/Database.h"

#include "GUI/Color Picker/Color Picker.h"

CObservedLootItem::CObservedLootItem(uintptr_t EntityAddress) : CBaseLootItem(EntityAddress)
{
	//std::println("[CObservedLootItem] Constructed with {0:X}", m_EntityAddress);
}

void CObservedLootItem::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseLootItem::PrepareRead_1(vmsh);

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CLootItem::pTemplateID, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_TarkovIDAddressPointer), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CLootItem::pItem, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ItemAddress), nullptr);
}

void CObservedLootItem::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseLootItem::PrepareRead_2(vmsh);

	VMMDLL_Scatter_PrepareEx(vmsh, m_TarkovIDAddressPointer + 0x14, sizeof(m_TarkovID), reinterpret_cast<BYTE*>(m_TarkovID.data()), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_ItemAddress + Offsets::CItem::pTemplate, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ItemTemplateAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_ItemAddress + Offsets::CItem::StackCount, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_StackCount), nullptr);
}

void CObservedLootItem::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseLootItem::PrepareRead_3(vmsh);

	m_pItemTemplate = std::make_unique<CItemTemplate>(m_ItemTemplateAddress);
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
	std::string TarkovIDStr = std::string(m_TarkovID.begin(), m_TarkovID.end());
	m_ItemPrice = TarkovItemData::GetPriceOfItem(TarkovIDStr);
	m_Name = TarkovItemData::GetShortNameOfItem(TarkovIDStr);

	if(m_ItemPrice == -1 && m_Name.empty())
		std::println("[CObservedLootItem] Failed to get item data for Tarkov ID: {0}", TarkovIDStr);
}

const ImColor& CObservedLootItem::GetRadarColor() const
{
	return ColorPicker::Radar::m_LootColor;
}

const ImColor& CObservedLootItem::GetFuserColor() const
{
	return ColorPicker::Fuser::m_LootColor;
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