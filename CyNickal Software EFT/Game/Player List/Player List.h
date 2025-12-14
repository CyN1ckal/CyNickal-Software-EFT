#pragma once
#include "Game/Classes/Players/CClientPlayer/CClientPlayer.h"
#include "Game/Classes/Players/CObservedPlayer/CObservedPlayer.h"
#include "DMA/DMA.h"

class PlayerList
{
public: /* Interface variables */
	static inline std::mutex m_PlayerMutex{};
	using Player = std::variant<CClientPlayer, CObservedPlayer>;
	static inline std::vector<Player> m_Players{};

public: /* Interface methods */
	static void QuickUpdate(DMA_Connection* Conn);
	static void FullUpdate(DMA_Connection* Conn);
	static void UpdateBaseAddresses(DMA_Connection* Conn, uintptr_t LocalGameWorld);
	static void HandlePlayerAllocations(DMA_Connection* Conn);
	static Vector3 GetLocalPlayerPosition();
	static Vector3 GetPlayerBonePosition(uintptr_t m_EntityAddress, EBoneIndex BoneIndex);
	static CClientPlayer* GetLocalPlayer();

private: /* Private methods */
	static void GetPlayerAddresses(DMA_Connection* Conn, std::vector<uintptr_t>& OutClientPlayers, std::vector<uintptr_t>& OutObservedPlayers);
	static void ExecuteReadsOnPlayerVec(DMA_Connection* Conn, std::vector<Player>& Players);
	static void AllocatePlayersFromVector(DMA_Connection* Conn, std::vector<uintptr_t> PlayerAddresses, EPlayerType playerType);
	static void DeallocatePlayersFromVector(std::vector<uintptr_t> PlayerAddresses, EPlayerType playerType);

private: /* Cache of addresses which are already processed */
	static inline std::vector<uintptr_t> m_PreviousObservedPlayers{};
	static inline std::vector<uintptr_t> m_PreviousClientPlayers{};
	static void AddPlayersToCache(std::vector<uintptr_t>& Addresses, EPlayerType PlayerType);
	static void RemoveAddressesFromCache(std::vector<uintptr_t>& Addresses, EPlayerType playerType);

private: /* Data read from the game */
	static inline uintptr_t m_RegisteredPlayersBaseAddress{ 0 };
	static inline uintptr_t m_PlayerDataBaseAddress{ 0 };
	static inline uint32_t m_NumPlayers{ 0 };
};