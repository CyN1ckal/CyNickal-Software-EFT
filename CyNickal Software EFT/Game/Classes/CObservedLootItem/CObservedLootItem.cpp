#include "pch.h"

#include "CObservedLootItem.h"

#include "Game/Offsets/Offsets.h"

#include "Database/Database.h"

#include "GUI/Color Picker/Color Picker.h"

CObservedLootItem::CObservedLootItem(uintptr_t EntityAddress) : CBaseLootItem(EntityAddress)
{
	std::println("[CObservedLootItem] Constructed with {0:X}", m_EntityAddress);
}

void CObservedLootItem::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseLootItem::PrepareRead_1(vmsh);

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CLootItem::pTemplateID, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_TarkovIDAddressPointer), nullptr);
}

void CObservedLootItem::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseLootItem::PrepareRead_2(vmsh);

	VMMDLL_Scatter_PrepareEx(vmsh, m_TarkovIDAddressPointer + 0x14, sizeof(m_TarkovID), reinterpret_cast<BYTE*>(m_TarkovID.data()), nullptr);
}

void CObservedLootItem::Finalize()
{
	CBaseLootItem::Finalize();

	std::string TarkovIDStr = std::string(m_TarkovID.begin(), m_TarkovID.end());
	m_ItemPrice = TarkovItemData::GetPriceOfItem(TarkovIDStr);
	m_Name = TarkovItemData::GetShortNameOfItem(TarkovIDStr);
}

const ImColor& CObservedLootItem::GetRadarColor() const
{
	return ColorPicker::Radar::m_LootColor;	
}

const ImColor& CObservedLootItem::GetFuserColor() const
{
	return ColorPicker::Fuser::m_LootColor;
}