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
	int32_t GetItemPrice() const { return m_ItemPrice; }
	const std::string& GetName() const { return m_Name; }
	const ImColor& GetRadarColor() const;
	const ImColor& GetFuserColor() const;
	const uint32_t GetSizeInSlots() const;
	const float GetPricePerSlot() const;
	const uint32_t GetStackCount() const { return m_StackCount; }

private:
	std::unique_ptr<CItemTemplate> m_pItemTemplate{ nullptr };
	std::string m_Name{""};
	uintptr_t m_TarkovIDAddressPointer{ 0 };
	uintptr_t m_ItemAddress{ 0 };
	uintptr_t m_ItemTemplateAddress{ 0 };
	int32_t m_ItemPrice{ -1 };
	uint32_t m_StackCount{ 0 };
	std::array<wchar_t, 24> m_TarkovID{};
};