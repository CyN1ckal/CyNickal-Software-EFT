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
	static bool Validate(DMA_Connection* Conn, std::unique_ptr<CLocalGameWorld>& pGameWorld);
	static void EnsureGameWorld(DMA_Connection* Conn, std::unique_ptr<CLocalGameWorld>& pGameWorld);

public:
	std::unique_ptr<class CLootList> m_pLootList{ nullptr };
	std::unique_ptr<class CRegisteredPlayers> m_pRegisteredPlayers{ nullptr };
	std::unique_ptr<class CExfilController> m_pExfilController{ nullptr };

private:
	std::uintptr_t LootListAddress{};
	std::uintptr_t RegisteredPlayersAddress{};
	std::uintptr_t ExfiltrationControllerAddress{};
};