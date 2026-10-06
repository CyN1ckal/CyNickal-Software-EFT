/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "DMA/CDMAConnection/CDMAConnection.h"
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Classes/Players/CClientPlayer/CClientPlayer.h"
#include "Game/Classes/Players/CObservedPlayer/CObservedPlayer.h"

class CRegisteredPlayers : public CBaseEntity
{
public:
	CRegisteredPlayers(uintptr_t RegisteredPlayersAddress);

public:
	using Player = std::variant<CClientPlayer, CObservedPlayer>;

	template <typename F>
	void ForEach_lk(F Func) {
		std::scoped_lock Lock(m_Mut);
		for (const auto& Player : m_Players) {
			Func(Player);
		}
	}

	template <typename F>
	void ForEach(F Func) {
		for (const auto& Player : m_Players) {
			Func(Player);
		}
	}

private:
	std::mutex m_Mut{};
	std::vector<Player> m_Players{};

public: /* Interface methods */
	void QuickUpdate(CDMAConnection* Conn);
	void FullUpdate(CDMAConnection* Conn, uintptr_t LocalPlayerAddress);
	void UpdateBaseAddresses(CDMAConnection* Conn);
	void HandlePlayerAllocations(CDMAConnection* Conn, uintptr_t LocalPlayerAddress);
	Vector3 GetLocalPlayerPosition();
	Vector3 GetPlayerBonePosition(uintptr_t m_EntityAddress, EBoneIndex BoneIndex);
	CClientPlayer* GetLocalPlayer();
	std::size_t GetNumValidPlayers();
	const bool IsLocalPlayerAiming();
	CShallowMagazine GetLocalPlayerMagazine();

private: /* Private methods */
	void GetPlayerAddresses(CDMAConnection* Conn, std::vector<uintptr_t>& OutClientPlayers, std::vector<uintptr_t>& OutObservedPlayers);
	void ExecuteReadsOnPlayerVec(CDMAConnection* Conn, std::vector<Player>& Players, uintptr_t LocalPlayerAddress);
	void AllocatePlayersFromVector(CDMAConnection* Conn, std::vector<uintptr_t> PlayerAddresses, EPlayerType playerType, uintptr_t LocalPlayerAddress);
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