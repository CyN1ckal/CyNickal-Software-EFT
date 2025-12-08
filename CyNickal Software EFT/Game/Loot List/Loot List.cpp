#include "pch.h"
#include "Loot List.h"
#include "Game/Offsets/Offsets.h"
#include "Game/EFT.h"

std::vector<uintptr_t> m_UnsortedLootAddresses{};
void LootList::CompleteUpdate(DMA_Connection* Conn, uintptr_t LocalGameWorld)
{
	auto& Proc = EFT::GetProcess();

	m_pLootListAddress = Proc.ReadMem<uintptr_t>(Conn, LocalGameWorld + Offsets::CLocalGameWorld::pLootList);

	m_BaseLootListAddress = Proc.ReadMem<uintptr_t>(Conn, m_pLootListAddress + 0x10);
	m_LootNum = Proc.ReadMem<uint32_t>(Conn, m_pLootListAddress + 0x18);
	m_LootMax = Proc.ReadMem<uint32_t>(Conn, m_pLootListAddress + 0x1C);

	std::println("[Loot List] @ {0:X} with {1:d}/{2:d}", m_BaseLootListAddress, m_LootNum, m_LootMax);

	m_UnsortedLootAddresses.clear();
	m_UnsortedLootAddresses.resize(m_LootNum);

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc.GetPID(), VMMDLL_FLAG_NOCACHE);
	VMMDLL_Scatter_PrepareEx(vmsh, m_BaseLootListAddress + 0x20, m_LootNum * sizeof(uintptr_t), reinterpret_cast<BYTE*>(m_UnsortedLootAddresses.data()), nullptr);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	FullUpdate(Conn);
}

void LootList::FullUpdate(DMA_Connection* Conn)
{
	auto& Proc = EFT::GetProcess();

	std::scoped_lock lk(m_LootMutex);

	m_LootList.clear();
	for (auto& Addr : m_UnsortedLootAddresses)
		m_LootList.emplace_back(CLoot(Addr));

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc.GetPID(), VMMDLL_FLAG_NOCACHE);
	for (auto& Loot : m_LootList)
		Loot.PrepareRead_1(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Loot : m_LootList)
		Loot.PrepareRead_2(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Loot : m_LootList)
		Loot.PrepareRead_3(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Loot : m_LootList)
		Loot.PrepareRead_4(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Loot : m_LootList)
		Loot.PrepareRead_5(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Loot : m_LootList)
		Loot.PrepareRead_6(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Loot : m_LootList)
		Loot.PrepareRead_7(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Loot : m_LootList)
		Loot.PrepareRead_8(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	VMMDLL_Scatter_CloseHandle(vmsh);

	for (auto& Loot : m_LootList)
		Loot.Finalize();
}

void LootList::QuickUpdate(DMA_Connection* Conn)
{
	std::scoped_lock lk(m_LootMutex);

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), EFT::GetProcess().GetPID(), VMMDLL_FLAG_NOCACHE);
	for (auto& Loot : m_LootList)
		Loot.QuickRead(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	for (auto& Loot : m_LootList)
		Loot.QuickFinalize();
}