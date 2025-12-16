#pragma once
#include "Game/Classes/CBaseLootItem/CBaseLootItem.h"

class CLootableContainer : public CBaseLootItem
{
public:
	CLootableContainer(uintptr_t EntityAddress);
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	const std::string& GetName() const { return m_Name; }
	const ImColor& GetRadarColor() const;
	const ImColor& GetFuserColor() const;

private:
	std::string m_Name{};
	std::array<wchar_t, 24> m_TarkovID{};
	uintptr_t m_TarkovIDAddressPointer{ 0 };
};