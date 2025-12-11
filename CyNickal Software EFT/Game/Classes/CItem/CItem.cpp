#include "pch.h"
#include "CItem.h"
#include "Game/Offsets/Offsets.h"
#include "DMA/DMA.h"
#include "Game/EFT.h"

CItem::CItem(uintptr_t EntityAddress) : CBaseEntity(EntityAddress)
{
	//std::println("[CItem] constructed {0:X}", m_EntityAddress);
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

	VMMDLL_Scatter_PrepareEx(vmsh, m_ItemTemplateAddress + Offsets::CItemTemplate::pName, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ItemTemplateNameAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CItem::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (!m_ItemTemplateNameAddress)
		SetInvalid();

	if (IsInvalid()) return;

	m_wItemNameBuffer.fill(0);
	VMMDLL_Scatter_PrepareEx(vmsh, m_ItemTemplateNameAddress + 0x14, sizeof(m_wItemNameBuffer), reinterpret_cast<BYTE*>(&m_wItemNameBuffer), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CItem::CompleteUpdate()
{
	auto Conn = DMA_Connection::GetInstance();
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

#include "Database/Database.h"
std::string BuiltName{};
void CItem::Finalize()
{
	if (m_BytesRead != sizeof(m_wItemNameBuffer))
		SetInvalid();

	if (IsInvalid()) return;

	for (int i = 0; i < 32; i++)
		m_ItemNameBuffer[i] = static_cast<char>(m_wItemNameBuffer[i]);

	BuiltName.clear();
	BuiltName = std::string(m_ItemNameBuffer.data(),32);
	m_ItemHash = CItemHash(BuiltName);
}

const char* CItem::GetUnfilteredName() const
{
	return m_ItemNameBuffer.data();
}

const char* CItem::GetSanitizedName() const
{
	return m_ItemHash.GetName().c_str();
}