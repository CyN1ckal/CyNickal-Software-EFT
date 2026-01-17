#pragma once
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Classes/CLootList/CLootList.h"
#include "Game/Classes/CRegisteredPlayers/CRegisteredPlayers.h"
#include "Game/Classes/CExfilController/CExfilController.h"

class CLocalGameWorld : public CBaseEntity
{
public:
	CLocalGameWorld(uintptr_t GameWorldAddress);

public:
	void QuickUpdatePlayers(DMA_Connection* Conn);
	void HandlePlayerAllocations(DMA_Connection* Conn);

public:
	bool IsValidRaid(DMA_Connection* Conn);
	uintptr_t GetMainPlayerAddress() const { return m_MainPlayerAddress; }

public:
	std::unique_ptr<class CLootList> m_pLootList{ nullptr };
	std::unique_ptr<class CRegisteredPlayers> m_pRegisteredPlayers{ nullptr };
	std::unique_ptr<class CExfilController> m_pExfilController{ nullptr };

private:
	uintptr_t m_MainPlayerAddress{ 0 };
	std::uintptr_t LootListAddress{};
	std::uintptr_t RegisteredPlayersAddress{};
	std::uintptr_t ExfiltrationControllerAddress{};
};