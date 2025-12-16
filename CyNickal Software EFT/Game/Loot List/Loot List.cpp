#include "pch.h"
#include "Loot List.h"
#include "Game/Offsets/Offsets.h"
#include "Game/EFT.h"

void LootList::CompleteUpdate(DMA_Connection* Conn)
{
	GetAndSortEntityAddresses(Conn);

	FullUpdate(Conn);
}

void LootList::FullUpdate(DMA_Connection* Conn)
{
	auto& Proc = EFT::GetProcess();

	std::scoped_lock lk(m_LootMutex);

	m_LootList.clear();
	for (auto& Addr : m_ObservedItemAddresses)
		m_LootList.emplace_back(CObservedLootItem(Addr));

	for (auto& Addr : m_LootableContainerAddresses)
		m_LootList.emplace_back(CLootableContainer(Addr));

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc.GetPID(), VMMDLL_FLAG_NOCACHE);
	for (auto& Loot : m_LootList)
		std::visit([&vmsh](auto& Loot) { Loot.PrepareRead_1(vmsh); }, Loot);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Loot : m_LootList)
		std::visit([&vmsh](auto& Loot) { Loot.PrepareRead_2(vmsh); }, Loot);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Loot : m_LootList)
		std::visit([&vmsh](auto& Loot) { Loot.PrepareRead_3(vmsh); }, Loot);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Loot : m_LootList)
		std::visit([&vmsh](auto& Loot) { Loot.PrepareRead_4(vmsh); }, Loot);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Loot : m_LootList)
		std::visit([&vmsh](auto& Loot) { Loot.PrepareRead_5(vmsh); }, Loot);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Loot : m_LootList)
		std::visit([&vmsh](auto& Loot) { Loot.PrepareRead_6(vmsh); }, Loot);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Loot : m_LootList)
		std::visit([&vmsh](auto& Loot) { Loot.PrepareRead_7(vmsh); }, Loot);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Loot : m_LootList)
		std::visit([&vmsh](auto& Loot) { Loot.PrepareRead_8(vmsh); }, Loot);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	VMMDLL_Scatter_CloseHandle(vmsh);

	for (auto& Loot : m_LootList)
		std::visit([](auto& Loot) { Loot.Finalize(); }, Loot);
}

void LootList::QuickUpdate(DMA_Connection* Conn)
{
	std::scoped_lock lk(m_LootMutex);

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), EFT::GetProcess().GetPID(), VMMDLL_FLAG_NOCACHE);
	for (auto& Loot : m_LootList)
		std::visit([&vmsh](auto& Loot) { Loot.QuickRead(vmsh); }, Loot);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	for (auto& Loot : m_LootList)
		std::visit([](auto& Loot) { Loot.QuickFinalize(); }, Loot);
}

std::vector<uintptr_t> DerefPointerVec(DMA_Connection* Conn, std::vector<uintptr_t>& Pointers)
{
	std::vector<uintptr_t> Return{};
	Return.resize(Pointers.size());

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), EFT::GetProcess().GetPID(), VMMDLL_FLAG_NOCACHE);
	for (auto&& [Index, Addr] : std::views::enumerate(Pointers))
	{
		if (Addr == 0)
			continue;

		VMMDLL_Scatter_PrepareEx(vmsh, Addr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&Return[Index]), nullptr);
	}
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	return Return;
}

std::vector<uintptr_t> ObjectClassAddr{};
void LootList::GetAndSortEntityAddresses(DMA_Connection* Conn)
{
	auto LocalGameWorld = EFT::GetCachedWorldAddress();
	auto& Proc = EFT::GetProcess();

	m_pLootListAddress = Proc.ReadMem<uintptr_t>(Conn, LocalGameWorld + Offsets::CLocalGameWorld::pLootList);

	m_BaseLootListAddress = Proc.ReadMem<uintptr_t>(Conn, m_pLootListAddress + 0x10);
	m_LootNum = Proc.ReadMem<uint32_t>(Conn, m_pLootListAddress + 0x18);
	m_UnsortedAddresses = Proc.ReadVec<uintptr_t>(Conn, m_BaseLootListAddress + 0x20, m_LootNum);

	if (ObjectTypeAddressCache.empty())
		PopulateTypeAddressCache(Conn);

	uintptr_t ObservedLootTypeAddress = ObjectTypeAddressCache["ObservedLootItem"];
	uintptr_t LootableContainerTypeAddress = ObjectTypeAddressCache["LootableContainer"];

	auto DerefLootAddresses = DerefPointerVec(Conn, m_UnsortedAddresses);

	m_ObservedItemAddresses.clear();
	m_LootableContainerAddresses.clear();
	for (auto&& [Index, Addr] : std::views::enumerate(DerefLootAddresses))
	{
		if (Addr == 0)
			continue;

		if (Addr == ObservedLootTypeAddress)
			m_ObservedItemAddresses.push_back(m_UnsortedAddresses[Index]);
		else if (Addr == LootableContainerTypeAddress)
			m_LootableContainerAddresses.push_back(m_UnsortedAddresses[Index]);
	}

	std::println("[Loot List] Found {} ObservedLootItems and {} LootableContainers", m_ObservedItemAddresses.size(), m_LootableContainerAddresses.size());
}

void LootList::PopulateTypeAddressCache(DMA_Connection* Conn)
{
	ObjectTypeAddressCache.clear();

	auto& Proc = EFT::GetProcess();

	auto DerefLootAddresses = DerefPointerVec(Conn, m_UnsortedAddresses);

	std::ranges::sort(DerefLootAddresses);
	auto ret = std::unique(DerefLootAddresses.begin(), DerefLootAddresses.end());
	DerefLootAddresses.erase(ret, DerefLootAddresses.end());

	std::vector<std::array<char, 64>> TypeNameBuffers{};
	TypeNameBuffers.resize(DerefLootAddresses.size());

	for (auto&& [Index, Addr] : std::views::enumerate(DerefLootAddresses))
	{
		if (Addr == 0)
			continue;

		uintptr_t NameAddr = Proc.ReadMem<uintptr_t>(Conn, Addr + 0x10);
		TypeNameBuffers[Index] = Proc.ReadMem<std::array<char, 64>>(Conn, NameAddr);
		std::string TypeNameStr = std::string(TypeNameBuffers[Index].begin(), TypeNameBuffers[Index].end());
		ObjectTypeAddressCache[TypeNameStr.c_str()] = Addr;
		std::println("[Loot List] Cached Type: {0} at {1:X}", TypeNameStr.c_str(), Addr);
	}
}