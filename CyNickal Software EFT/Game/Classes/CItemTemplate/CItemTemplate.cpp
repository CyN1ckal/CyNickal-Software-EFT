#include "pch.h"
#include "CItemTemplate.h"
#include "Game/Offsets/Offsets.h"

CItemTemplate::CItemTemplate(uintptr_t TemplateAddress)
	: CBaseEntity(TemplateAddress)
{
	//std::println("[CItemTemplate] Constructed at {:X}", TemplateAddress);
}

void CItemTemplate::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CItemTemplate::pName, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_NameAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CItemTemplate::pTarkovID, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_TarkovIDAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CItemTemplate::Width, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_Width), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CItemTemplate::Height, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_Height), nullptr);
}

void CItemTemplate::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (!m_NameAddress)
		SetInvalid();

	if (!m_TarkovIDAddress)
		SetInvalid();

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_NameAddress + 0x14, sizeof(m_wNameBuffer), reinterpret_cast<BYTE*>(m_wNameBuffer.data()), reinterpret_cast<DWORD*>(&m_BytesRead));
	VMMDLL_Scatter_PrepareEx(vmsh, m_TarkovIDAddress + 0x14, sizeof(m_wTarkovIDBuffer), reinterpret_cast<BYTE*>(m_wTarkovIDBuffer.data()), nullptr);
}

void CItemTemplate::Finalize()
{
	if (m_BytesRead != sizeof(m_wNameBuffer))
		SetInvalid();

	if (IsInvalid()) return;

	m_sName = std::string(m_wNameBuffer.begin(), m_wNameBuffer.end());
	m_sTarkovID = std::string(m_wTarkovIDBuffer.begin(), m_wTarkovIDBuffer.end());
}
