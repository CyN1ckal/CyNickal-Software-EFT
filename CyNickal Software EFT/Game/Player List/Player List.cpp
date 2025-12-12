#include "pch.h"
#include "Player List.h"
#include "Game/EFT.h"
#include "Game/Offsets/Offsets.h"
#include <algorithm>

void PlayerList::ExecuteReadsOnPlayerVec(DMA_Connection* Conn, std::vector<Player>& Players)
{
	const auto PID = EFT::GetProcess().GetPID();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_1(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_2(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_3(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_4(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_5(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_6(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_7(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_8(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_9(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Player : Players)
		std::visit([vmsh](auto& p) { p.PrepareRead_10(vmsh); }, Player);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	for (auto& Player : Players)
		std::visit([](auto& p) { p.Finalize(); }, Player);
}

void PlayerList::AddPlayersToCache(std::vector<uintptr_t>& Addresses, EPlayerType PlayerType)
{
	if (PlayerType == EPlayerType::eMainPlayer)
	{
		std::println("[PlayerList] Adding {} Client players to cache.", Addresses.size());
		m_PreviousClientPlayers.insert(m_PreviousClientPlayers.end(), Addresses.begin(), Addresses.end());
		std::ranges::sort(m_PreviousClientPlayers);
		std::println("[PlayerList] Cache total: {} client players", m_PreviousClientPlayers.size());
	}
	else if (PlayerType == EPlayerType::eObservedPlayer)
	{
		std::println("[PlayerList] Adding {} Observed players to cache.", Addresses.size());
		m_PreviousObservedPlayers.insert(m_PreviousObservedPlayers.end(), Addresses.begin(), Addresses.end());
		std::ranges::sort(m_PreviousObservedPlayers);
		std::println("[PlayerList] Cache total: {} observed players", m_PreviousObservedPlayers.size());
	}
}

void PlayerList::RemoveAddressesFromCache(std::vector<uintptr_t>& Addresses, EPlayerType playerType)
{
	if (playerType == EPlayerType::eMainPlayer)
	{
		for (auto& Addr : Addresses)
		{
			auto it = std::find(m_PreviousClientPlayers.begin(), m_PreviousClientPlayers.end(), Addr);
			if (it != m_PreviousClientPlayers.end())
			{
				m_PreviousClientPlayers.erase(it);
				std::println("[PlayerList] Removed address 0x{0:X} from Client cache", Addr);
			}
		}
	}
	else if (playerType == EPlayerType::eObservedPlayer)
	{
		for (auto& Addr : Addresses)
		{
			auto it = std::find(m_PreviousObservedPlayers.begin(), m_PreviousObservedPlayers.end(), Addr);
			if (it != m_PreviousObservedPlayers.end())
			{
				m_PreviousObservedPlayers.erase(it);
				std::println("[PlayerList] Removed address 0x{0:X} from Observed cache", Addr);
			}
		}
	}
}

void PlayerList::QuickUpdate(DMA_Connection* Conn)
{
	std::scoped_lock Lock(m_PlayerMutex);

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), EFT::GetProcess().GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
		std::visit([vmsh](auto& p) { p.QuickRead(vmsh); }, Player);

	VMMDLL_Scatter_Execute(vmsh);

	for (auto& Player : m_Players)
		std::visit([](auto& p) { p.QuickFinalize(); }, Player);

	VMMDLL_Scatter_CloseHandle(vmsh);
}

void PlayerList::FullUpdate(DMA_Connection* Conn)
{
	std::println("[PlayerList] Full update requested.");

	Conn->LightRefresh();

	std::scoped_lock Lock(m_PlayerMutex);
	ExecuteReadsOnPlayerVec(Conn, m_Players);
}

void PlayerList::UpdateBaseAddresses(DMA_Connection* Conn, uintptr_t LocalGameWorld)
{
	if (!LocalGameWorld)
	{
		m_RegisteredPlayersBaseAddress = 0x0;
		m_PlayerDataBaseAddress = 0x0;
		m_NumPlayers = 0;
		return;
	}

	uintptr_t PlayerListAddress = LocalGameWorld + Offsets::CLocalGameWorld::pRegisteredPlayers;

	auto& Proc = EFT::GetProcess();

	m_RegisteredPlayersBaseAddress = Proc.ReadMem<uintptr_t>(Conn, PlayerListAddress);

	m_PlayerDataBaseAddress = Proc.ReadMem<uintptr_t>(Conn, m_RegisteredPlayersBaseAddress + Offsets::CRegisteredPlayers::pPlayerArray);

	uintptr_t NumPlayersAddress = m_RegisteredPlayersBaseAddress + Offsets::CRegisteredPlayers::NumPlayers;
	uintptr_t MaxPlayersAddress = m_RegisteredPlayersBaseAddress + Offsets::CRegisteredPlayers::MaxPlayers;

	m_NumPlayers = Proc.ReadMem<uint32_t>(Conn, NumPlayersAddress);
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
void PlayerList::GetPlayerAddresses(DMA_Connection* Conn, std::vector<uintptr_t>& OutClientPlayers, std::vector<uintptr_t>& OutObservedPlayers)
{
	OutClientPlayers.clear();
	OutObservedPlayers.clear();

	if (!m_NumPlayers || !m_PlayerDataBaseAddress) return;

	auto& Proc = EFT::GetProcess();

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

			std::println("Object {0:X} is type '{1:s}'", ObjAddress, ObjName.Name);
			NameMap[ObjAddress] = std::string(ObjName.Name);
		}
	}

	VMMDLL_Scatter_CloseHandle(vmsh);

	for (int i = 0; i < PlayerInfos.size(); i++)
	{
		auto& PlayerInfo = PlayerInfos[i];

		if (PlayerInfo.ObjectAddress == 0) continue;

		if (NameMap[PlayerInfo.ObjectAddress] == "LocalPlayer" || NameMap[PlayerInfo.ObjectAddress] == "ClientPlayer")
			OutClientPlayers.push_back(PlayerInfo.PlayerAddress);
		else
			OutObservedPlayers.push_back(PlayerInfo.PlayerAddress);
	}
}

Vector3 PlayerList::GetLocalPlayerPosition()
{
	std::scoped_lock Lock(m_PlayerMutex);

	Vector3 ReturnPosition{};
	bool bFound{ false };

	for (auto& Player : m_Players)
	{
		std::visit([&](auto& p) {
			if (p.IsLocalPlayer()) {
				bFound = true;
				ReturnPosition = p.GetBonePosition(EBoneIndex::Root);
			}
			}, Player);

		if (bFound) break;
	}

	return ReturnPosition;
}

Vector3 PlayerList::GetPlayerPosition(uintptr_t m_EntityAddress, EBoneIndex BoneIndex)
{
	std::scoped_lock Lock(m_PlayerMutex);
	Vector3 BonePos{};
	bool bFound{ false };

	for (auto& Player : m_Players)
	{
		std::visit([&](auto& p) {
			if (p.m_EntityAddress == m_EntityAddress) {
				bFound = true;
				BonePos = p.GetBonePosition(BoneIndex);
			}
			}, Player);

		if (bFound)	return BonePos;
	}
	return BonePos;
}

void PlayerList::AllocatePlayersFromVector(DMA_Connection* Conn, std::vector<uintptr_t> PlayerAddresses, EPlayerType playerType)
{
	std::println("[PlayerList] Allocating {} players of type {}", PlayerAddresses.size(),
		(playerType == EPlayerType::eMainPlayer) ? "ClientPlayer" : "ObservedPlayer");

	std::vector<Player> m_LocalCopy{};

	for (auto& Addr : PlayerAddresses)
	{
		if (playerType == EPlayerType::eMainPlayer)
			m_LocalCopy.emplace_back(CClientPlayer(Addr));
		else if (playerType == EPlayerType::eObservedPlayer)
			m_LocalCopy.emplace_back(CObservedPlayer(Addr));
	}

	ExecuteReadsOnPlayerVec(Conn, m_LocalCopy);

	std::scoped_lock Lock(m_PlayerMutex);
	m_Players.insert(m_Players.end(),
		std::make_move_iterator(m_LocalCopy.begin()),
		std::make_move_iterator(m_LocalCopy.end()));

	AddPlayersToCache(PlayerAddresses, playerType);
}

void PlayerList::DeallocatePlayersFromVector(std::vector<uintptr_t> PlayerAddresses, EPlayerType playerType)
{
	std::scoped_lock Lock(m_PlayerMutex);
	for (auto& Addr : PlayerAddresses)
	{
		auto it = std::find_if(m_Players.begin(), m_Players.end(), [Addr](const Player& p) {
			return std::visit([Addr](auto& player) {
				return player.m_EntityAddress == Addr;
				}, p);
			});

		if (it != m_Players.end())
		{
			m_Players.erase(it);
			std::println("[PlayerList] Deallocated player at address 0x{0:X}", Addr);
		}
	}

	RemoveAddressesFromCache(PlayerAddresses, playerType);
}

std::vector<uintptr_t> NewClientPlayers{};
std::vector<uintptr_t> NewObservedPlayers{};
std::vector<uintptr_t> OutdatedClients{};
std::vector<uintptr_t> OutdatedObservers{};
std::vector<uintptr_t> All_ClientPlayers{};
std::vector<uintptr_t> All_ObservedPlayers{};
void PlayerList::HandlePlayerAllocations(DMA_Connection* Conn)
{
	GetPlayerAddresses(Conn, All_ClientPlayers, All_ObservedPlayers);

	std::ranges::sort(All_ClientPlayers);
	std::ranges::sort(All_ObservedPlayers);

	NewClientPlayers.clear();
	std::set_difference(All_ClientPlayers.begin(), All_ClientPlayers.end(),
		m_PreviousClientPlayers.begin(), m_PreviousClientPlayers.end(),
		std::back_inserter(NewClientPlayers));

	NewObservedPlayers.clear();
	std::set_difference(All_ObservedPlayers.begin(), All_ObservedPlayers.end(),
		m_PreviousObservedPlayers.begin(), m_PreviousObservedPlayers.end(),
		std::back_inserter(NewObservedPlayers));

	if (NewClientPlayers.size())
		AllocatePlayersFromVector(Conn, NewClientPlayers, EPlayerType::eMainPlayer);
	if (NewObservedPlayers.size())
		AllocatePlayersFromVector(Conn, NewObservedPlayers, EPlayerType::eObservedPlayer);

	OutdatedClients.clear();
	std::set_difference(m_PreviousClientPlayers.begin(), m_PreviousClientPlayers.end(),
		All_ClientPlayers.begin(), All_ClientPlayers.end(),
		std::back_inserter(OutdatedClients));

	OutdatedObservers.clear();
	std::set_difference(m_PreviousObservedPlayers.begin(), m_PreviousObservedPlayers.end(),
		All_ObservedPlayers.begin(), All_ObservedPlayers.end(),
		std::back_inserter(OutdatedObservers));

	if (OutdatedClients.size())
		DeallocatePlayersFromVector(OutdatedClients, EPlayerType::eMainPlayer);
	if (OutdatedObservers.size())
		DeallocatePlayersFromVector(OutdatedObservers, EPlayerType::eObservedPlayer);
}