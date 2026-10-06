/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CLootList.h"

CLootList::CLootList(uintptr_t LootListAddress) : CBaseEntity(LootListAddress)
{
	ZoneScoped;

	std::println("[CLootList] Constructed CLootList with 0x{:X}", LootListAddress);

	auto Conn = CDMAConnection::GetInstance();
	CompleteUpdate(Conn);

	std::println("[CLootList] CLootList initialized with {} items and {} containers.", m_ObservedLootItemAddresses.size(), m_LootableContainerAddresses.size());
}

void CLootList::QuickUpdate(CDMAConnection* Conn) {
	ZoneScoped;

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), EFT::GetProcess().GetPID(), VMMDLL_FLAG_NOCACHE);

	m_Items.ForEach([vmsh](CObservedLootItem& Item) { Item.QuickRead(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	m_Items.ForEach([vmsh](CObservedLootItem& Item) { Item.QuickFinalize(); });
}

void CLootList::CompleteUpdate(CDMAConnection* Conn)
{
	ZoneScoped;

	GetAndSortEntityAddresses(Conn);

	std::scoped_lock Lock(m_Items.m_Mutex, m_Containers.m_Mutex, m_Corpses.m_Mutex);

	m_Containers.HandleAllocations(m_LootableContainerAddresses);
	m_Items.HandleAllocations(m_ObservedLootItemAddresses);
	m_Corpses.HandleAllocations(m_CorpseAddresses);

	auto PID = EFT::GetProcess().GetPID();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), PID, VMMDLL_FLAG_NOCACHE);
	m_Containers.ForEach([vmsh](CLootableContainer& Container) { Container.PrepareRead_1(vmsh); });
	m_Items.ForEach([vmsh](CObservedLootItem& Item) { Item.PrepareRead_1(vmsh); });
	m_Corpses.ForEach([vmsh](CCorpse& Corpse) { Corpse.PrepareRead_1(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);
	m_Containers.ForEach([vmsh](CLootableContainer& Container) { Container.PrepareRead_2(vmsh); });
	m_Items.ForEach([vmsh](CObservedLootItem& Item) { Item.PrepareRead_2(vmsh); });
	m_Corpses.ForEach([vmsh](CCorpse& Corpse) { Corpse.PrepareRead_2(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);
	m_Containers.ForEach([vmsh](CLootableContainer& Container) { Container.PrepareRead_3(vmsh); });
	m_Items.ForEach([vmsh](CObservedLootItem& Item) { Item.PrepareRead_3(vmsh); });
	m_Corpses.ForEach([vmsh](CCorpse& Corpse) { Corpse.PrepareRead_3(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);
	m_Containers.ForEach([vmsh](CLootableContainer& Container) { Container.PrepareRead_4(vmsh); });
	m_Items.ForEach([vmsh](CObservedLootItem& Item) { Item.PrepareRead_4(vmsh); });
	m_Corpses.ForEach([vmsh](CCorpse& Corpse) { Corpse.PrepareRead_4(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);
	m_Containers.ForEach([vmsh](CLootableContainer& Container) { Container.PrepareRead_5(vmsh); });
	m_Items.ForEach([vmsh](CObservedLootItem& Item) { Item.PrepareRead_5(vmsh); });
	m_Corpses.ForEach([vmsh](CCorpse& Corpse) { Corpse.PrepareRead_5(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);
	m_Containers.ForEach([vmsh](CLootableContainer& Container) { Container.PrepareRead_6(vmsh); });
	m_Items.ForEach([vmsh](CObservedLootItem& Item) { Item.PrepareRead_6(vmsh); });
	m_Corpses.ForEach([vmsh](CCorpse& Corpse) { Corpse.PrepareRead_6(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);
	m_Containers.ForEach([vmsh](CLootableContainer& Container) { Container.PrepareRead_7(vmsh); });
	m_Items.ForEach([vmsh](CObservedLootItem& Item) { Item.PrepareRead_7(vmsh); });
	m_Corpses.ForEach([vmsh](CCorpse& Corpse) { Corpse.PrepareRead_7(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);
	m_Containers.ForEach([vmsh](CLootableContainer& Container) { Container.PrepareRead_8(vmsh); });
	m_Items.ForEach([vmsh](CObservedLootItem& Item) { Item.PrepareRead_8(vmsh); });
	m_Corpses.ForEach([vmsh](CCorpse& Corpse) { Corpse.PrepareRead_8(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	m_Containers.ForEach([](CLootableContainer& Container) { Container.Finalize(); });
	m_Items.ForEach([](CObservedLootItem& Item) { Item.Finalize(); });
	m_Corpses.ForEach([](CCorpse& Corpse) { Corpse.Finalize(); });
}

std::vector<uintptr_t> DerefPointerVec(CDMAConnection* Conn, std::vector<uintptr_t>& Pointers)
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

void CLootList::GetAndSortEntityAddresses(CDMAConnection* Conn)
{
	ZoneScoped;

	auto& Proc = EFT::GetProcess();

	m_BaseLootListAddress = Proc.ReadMem<uintptr_t>(Conn, m_EntityAddress + 0x10);
	m_LootNum = Proc.ReadMem<uint32_t>(Conn, m_EntityAddress + 0x18);
	m_UnsortedAddresses = Proc.ReadVec<uintptr_t>(Conn, m_BaseLootListAddress + 0x20, m_LootNum);

	PopulateTypeAddressCache(Conn);

	uintptr_t ObservedLootTypeAddress = ObjectTypeAddressCache.at("ObservedLootItem");
	uintptr_t LootableContainerTypeAddress = ObjectTypeAddressCache.at("LootableContainer");
	uintptr_t CorpseTypeAddress = 0;
	uintptr_t ObservedCorpseTypeAddress = 0;
	if (ObjectTypeAddressCache.contains("ObservedCorpse")) ObservedCorpseTypeAddress = ObjectTypeAddressCache.at("ObservedCorpse");
	if (ObjectTypeAddressCache.contains("Corpse")) CorpseTypeAddress = ObjectTypeAddressCache.at("Corpse");

	auto DerefLootAddresses = DerefPointerVec(Conn, m_UnsortedAddresses);

	m_ObservedLootItemAddresses.clear();
	m_LootableContainerAddresses.clear();
	m_CorpseAddresses.clear();
	for (auto&& [Index, Addr] : std::views::enumerate(DerefLootAddresses))
	{
		if (Addr == 0)
			continue;

		if (Addr == ObservedLootTypeAddress)
			m_ObservedLootItemAddresses.push_back(m_UnsortedAddresses[Index]);
		else if (Addr == LootableContainerTypeAddress)
			m_LootableContainerAddresses.push_back(m_UnsortedAddresses[Index]);
		else if (CorpseTypeAddress && Addr == CorpseTypeAddress)
			m_CorpseAddresses.push_back(m_UnsortedAddresses[Index]);
		else if (ObservedCorpseTypeAddress && Addr == ObservedCorpseTypeAddress)
			m_CorpseAddresses.push_back(m_UnsortedAddresses[Index]);
	}

	std::println("[CLootList] Found: \n   {} ObservedLootItems\n   {} LootableContainers\n   {} Corpses", m_ObservedLootItemAddresses.size(), m_LootableContainerAddresses.size(), m_CorpseAddresses.size());
}

void CLootList::PopulateTypeAddressCache(CDMAConnection* Conn)
{
	ZoneScoped;

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
		std::println("[CLootList] Cached Type: {0} at {1:X}", TypeNameStr.c_str(), Addr);
	}
}