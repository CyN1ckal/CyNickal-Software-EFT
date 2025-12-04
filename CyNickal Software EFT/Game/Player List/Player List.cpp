#include "pch.h"
#include "Player List.h"
#include "Game/EFT.h"
#include "Game/Offsets/Offsets.h"

void PlayerList::FullUpdate(DMA_Connection* Conn)
{
	std::println("[Player List] Performing full update...");

	std::scoped_lock Lock(m_PlayerMutex);

	m_Players.clear();

	const auto PID = EFT::GetProcess().GetPID();

	for (auto& PlayerAddr : m_PlayerAddr)
		m_Players.emplace_back(CEFTPlayer(PlayerAddr));

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		Player.PrepareRead_1(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		Player.PrepareRead_2(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		Player.PrepareRead_3(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		Player.PrepareRead_4(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		Player.PrepareRead_5(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		Player.PrepareRead_6(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		Player.PrepareRead_7(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		Player.PrepareRead_8(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	for (auto& Player : m_Players)
		Player.Finalize();

	std::println("[Player List] Full update complete. {} players.", m_Players.size());
}

void PlayerList::QuickUpdate(DMA_Connection* Conn)
{
	std::scoped_lock Lock(m_PlayerMutex);

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), EFT::GetProcess().GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		Player.QuickRead(vmsh);

	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	for (auto& Player : m_Players)
		Player.QuickFinalize();
}

void PlayerList::PrintPlayers()
{
	std::scoped_lock Lock(m_PlayerMutex);
	for (int i = 0; i < m_Players.size(); i++)
	{
		auto& Player = m_Players[i];
		if (Player.IsInvalid()) continue;
		std::println("Player #{0:d} @ 0x{1:X} - Base Position: X: {2:f}, Y: {3:f}, Z: {4:f}", i, Player.m_EntityAddress, Player.m_BasePosition.x, Player.m_BasePosition.y, Player.m_BasePosition.z);
	}
	std::println("");
}

void PlayerList::CompleteUpdate(DMA_Connection* Conn, uintptr_t LocalGameWorld)
{
	UpdateBaseAddresses(Conn, LocalGameWorld);
	PopulatePlayerAddresses(Conn);
	FullUpdate(Conn);
}

void PlayerList::UpdateBaseAddresses(DMA_Connection* Conn, uintptr_t LocalGameWorld)
{
	uintptr_t PlayerListAddress = LocalGameWorld + Offsets::CLocalGameWorld::pRegisteredPlayers;

	auto& Proc = EFT::GetProcess();

	m_RegisteredPlayersBaseAddress = Proc.ReadMem<uintptr_t>(Conn, PlayerListAddress);
	std::println("[Player List] Registered Players Address: 0x{:X}", m_RegisteredPlayersBaseAddress);

	m_PlayerDataBaseAddress = Proc.ReadMem<uintptr_t>(Conn, m_RegisteredPlayersBaseAddress + Offsets::CRegisteredPlayers::pPlayerArray);
	std::println("[Player List] Player Data Address: 0x{:X}", m_PlayerDataBaseAddress);

	uintptr_t NumPlayersAddress = m_RegisteredPlayersBaseAddress + Offsets::CRegisteredPlayers::NumPlayers;
	uintptr_t MaxPlayersAddress = m_RegisteredPlayersBaseAddress + Offsets::CRegisteredPlayers::MaxPlayers;

	m_NumPlayers = Proc.ReadMem<uint32_t>(Conn, NumPlayersAddress);
	m_MaxPlayers = Proc.ReadMem<uint32_t>(Conn, MaxPlayersAddress);
	std::println("[Player List] {}/{}", m_NumPlayers, m_MaxPlayers);
}

void PlayerList::PopulatePlayerAddresses(DMA_Connection* Conn)
{
	auto& Proc = EFT::GetProcess();

	m_PlayerAddr.clear();
	m_PlayerAddr.resize(m_NumPlayers);

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc.GetPID(), VMMDLL_FLAG_NOCACHE);
	VMMDLL_Scatter_PrepareEx(vmsh, m_PlayerDataBaseAddress + 0x20, sizeof(uintptr_t) * m_NumPlayers, reinterpret_cast<BYTE*>(m_PlayerAddr.data()), nullptr);
	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);
}