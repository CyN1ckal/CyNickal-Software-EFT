#pragma once
#include "DMA/DMA.h"
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Classes/Players/CClientPlayer/CClientPlayer.h"
#include "Game/Classes/Players/CObservedPlayer/CObservedPlayer.h"

class CRegisteredPlayers : public CBaseEntity
{
public:
	CRegisteredPlayers(uintptr_t RegisteredPlayersAddress);

public: /* Interface variables */
	using Player = std::variant<CClientPlayer, CObservedPlayer>;
	std::mutex m_Mut{};
	std::vector<Player> m_Players{};

public: /* Interface methods */
	void QuickUpdate(DMA_Connection* Conn);
	void FullUpdate(DMA_Connection* Conn);
	void UpdateBaseAddresses(DMA_Connection* Conn);
	void HandlePlayerAllocations(DMA_Connection* Conn);
	Vector3 GetLocalPlayerPosition();
	Vector3 GetPlayerBonePosition(uintptr_t m_EntityAddress, EBoneIndex BoneIndex);
	CClientPlayer* GetLocalPlayer();
	std::size_t GetNumValidPlayers();

private: /* Private methods */
	void GetPlayerAddresses(DMA_Connection* Conn, std::vector<uintptr_t>& OutClientPlayers, std::vector<uintptr_t>& OutObservedPlayers);
	void ExecuteReadsOnPlayerVec(DMA_Connection* Conn, std::vector<Player>& Players);
	void AllocatePlayersFromVector(DMA_Connection* Conn, std::vector<uintptr_t> PlayerAddresses, EPlayerType playerType);
	void DeallocatePlayersFromVector(std::vector<uintptr_t> PlayerAddresses, EPlayerType playerType);

private: /* Cache of addresses which are already processed */
	std::vector<uintptr_t> m_PreviousObservedPlayers{};
	std::vector<uintptr_t> m_PreviousClientPlayers{};
	void AddPlayersToCache(std::vector<uintptr_t>& Addresses, EPlayerType PlayerType);
	void RemoveAddressesFromCache(std::vector<uintptr_t>& Addresses, EPlayerType playerType);

private: /* Data read from the game */
	uintptr_t m_PlayerDataBaseAddress{ 0 };
	uint32_t m_NumPlayers{ 0 };
};