#include "pch.h"
#include "Player List.h"
#include "Game/EFT.h"
#include "Game/Offsets/Offsets.h"
#include <algorithm>

void PlayerList::FullUpdate(DMA_Connection* Conn)
{
	std::scoped_lock Lock(m_PlayerMutex);

	m_Players.clear();

	const auto PID = EFT::GetProcess().GetPID();

	for (auto& Addr : m_ClientPlayerAddresses)
		m_Players.emplace_back(CClientPlayer(Addr));

	for (auto& Addr : m_ObservedPlayerAddresses)
		m_Players.emplace_back(CObservedPlayer(Addr));

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_1(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_2(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_3(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_4(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_5(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_6(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_7(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_8(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	for (auto& Player : m_Players)
		std::visit([](auto& p) { p.Finalize(); }, Player);
}

void PlayerList::QuickUpdate(DMA_Connection* Conn)
{
	std::scoped_lock Lock(m_PlayerMutex);

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), EFT::GetProcess().GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		std::visit([vmsh](auto& p) { p.QuickRead(vmsh); }, Player);

	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	for (auto& Player : m_Players)
		std::visit([](auto& p) { p.QuickFinalize(); }, Player);
}

void PlayerList::PrintPlayers()
{
	std::scoped_lock Lock(m_PlayerMutex);
	//for (int i = 0; i < m_Players.size(); i++)
	//{
	//	auto& Player = m_Players[i];
	//	if (Player.IsInvalid()) continue;
	//	std::println("Player #{0:d} @ 0x{1:X} - Base Position: X: {2:f}, Y: {3:f}, Z: {4:f}", i, Player.m_EntityAddress, Player.m_BasePosition.x, Player.m_BasePosition.y, Player.m_BasePosition.z);
	//}
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

	m_PlayerDataBaseAddress = Proc.ReadMem<uintptr_t>(Conn, m_RegisteredPlayersBaseAddress + Offsets::CRegisteredPlayers::pPlayerArray);

	uintptr_t NumPlayersAddress = m_RegisteredPlayersBaseAddress + Offsets::CRegisteredPlayers::NumPlayers;
	uintptr_t MaxPlayersAddress = m_RegisteredPlayersBaseAddress + Offsets::CRegisteredPlayers::MaxPlayers;

	m_NumPlayers = Proc.ReadMem<uint32_t>(Conn, NumPlayersAddress);
	m_MaxPlayers = Proc.ReadMem<uint32_t>(Conn, MaxPlayersAddress);
}

struct PlayerInfo
{
	uintptr_t PlayerAddress{ 0 };
	uintptr_t ObjectAddress{ 0 };
};
std::vector<PlayerInfo> PlayerInfos{};
std::vector<uintptr_t> UniqueObjectAddresses{};
struct NameInfo
{
	uintptr_t NameAddress{ 0 };
	char Name[64]{ 0 };
};
std::vector<NameInfo> ObjectNames{};
std::unordered_map<uintptr_t, std::string> NameMap{};
void PlayerList::PopulatePlayerAddresses(DMA_Connection* Conn)
{
	auto& Proc = EFT::GetProcess();

	m_ClientPlayerAddresses.clear();
	m_ObservedPlayerAddresses.clear();

	PlayerInfos.clear();
	PlayerInfos.resize(m_NumPlayers);

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc.GetPID(), VMMDLL_FLAG_NOCACHE);
	for (int i = 0; i < m_NumPlayers; i++)
	{
		uintptr_t PlayerPtrAddress = m_PlayerDataBaseAddress + 0x20 + (sizeof(uintptr_t) * i);
		VMMDLL_Scatter_PrepareEx(vmsh, PlayerPtrAddress, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&PlayerInfos[i].PlayerAddress), nullptr);
	}
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (int i = 0; i < m_NumPlayers; i++)
	{
		auto& PlayerInfo = PlayerInfos[i];
		VMMDLL_Scatter_PrepareEx(vmsh, PlayerInfo.PlayerAddress, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&PlayerInfo.ObjectAddress), nullptr);
	}
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	if (NameMap.empty())
	{
		UniqueObjectAddresses.clear();
		for (auto& PlayerInfo : PlayerInfos)
		{
			if (std::find(UniqueObjectAddresses.begin(), UniqueObjectAddresses.end(), PlayerInfo.ObjectAddress) != UniqueObjectAddresses.end())
				continue;

			UniqueObjectAddresses.push_back(PlayerInfo.ObjectAddress);
		}

		ObjectNames.resize(UniqueObjectAddresses.size());
		for (int i = 0; i < UniqueObjectAddresses.size(); i++)
		{
			auto& ObjAddr = UniqueObjectAddresses[i];
			auto& NameInfoRef = ObjectNames[i];
			VMMDLL_Scatter_PrepareEx(vmsh, ObjAddr + 0x10, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&NameInfoRef.NameAddress), nullptr);
		}
		VMMDLL_Scatter_Execute(vmsh);
		VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

		for (int i = 0; i < ObjectNames.size(); i++)
		{
			auto& NameInfoRef = ObjectNames[i];
			VMMDLL_Scatter_PrepareEx(vmsh, NameInfoRef.NameAddress, sizeof(NameInfoRef.Name), reinterpret_cast<BYTE*>(&NameInfoRef.Name), nullptr);
		}
		VMMDLL_Scatter_Execute(vmsh);

		NameMap.clear();

		for (int i = 0; i < UniqueObjectAddresses.size(); i++)
		{
			auto& ObjName = ObjectNames[i];
			auto& ObjAddress = UniqueObjectAddresses[i];
			NameMap[ObjAddress] = std::string(ObjName.Name);
		}
	}

	VMMDLL_Scatter_CloseHandle(vmsh);

	for (int i = 0; i < PlayerInfos.size(); i++)
	{
		auto& PlayerInfo = PlayerInfos[i];

		if (NameMap[PlayerInfo.ObjectAddress] == "LocalPlayer" || NameMap[PlayerInfo.ObjectAddress] == "ClientPlayer")
			m_ClientPlayerAddresses.push_back(PlayerInfo.PlayerAddress);
		else
			m_ObservedPlayerAddresses.push_back(PlayerInfo.PlayerAddress);
	}
}