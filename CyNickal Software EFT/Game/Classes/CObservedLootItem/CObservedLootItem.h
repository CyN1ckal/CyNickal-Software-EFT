#pragma once
#include "Game/Classes/CBaseLootItem/CBaseLootItem.h"

class CObservedLootItem : public CBaseLootItem
{
public:
	CObservedLootItem(uintptr_t EntityAddress);
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	int32_t GetItemPrice() const { return m_ItemPrice; }
	const std::string& GetName() const { return m_Name; }
	const ImColor& GetRadarColor() const;
	const ImColor& GetFuserColor() const;

private:
	std::string m_Name{""};
	uintptr_t m_TarkovIDAddressPointer{ 0 };
	int32_t m_ItemPrice{ -1 };
	std::array<wchar_t, 24> m_TarkovID{};
};