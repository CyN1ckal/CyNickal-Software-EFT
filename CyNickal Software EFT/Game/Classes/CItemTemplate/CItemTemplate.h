#pragma once

#include "Game/Classes/CBaseEntity/CBaseEntity.h"

class CItemTemplate : public CBaseEntity
{
public:
	CItemTemplate(uintptr_t TemplateAddress);
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
	uint32_t GetSizeInSlots() const { return m_Width * m_Height; }

public:
	std::string m_sName{ "" };
	std::string m_sTarkovID{ "" };

private:
	std::array<wchar_t, 64>m_wNameBuffer{};
	std::array<wchar_t, 24>m_wTarkovIDBuffer{};
	uint32_t m_Width{ 0 };
	uint32_t m_Height{ 0 };
	uintptr_t m_NameAddress{ 0 };
	uintptr_t m_TarkovIDAddress{ 0 };
};