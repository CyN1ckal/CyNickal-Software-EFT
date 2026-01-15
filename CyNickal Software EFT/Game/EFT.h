#pragma once
#include "DMA/DMA.h"
#include "DMA/Process.h"
#include "Classes/CLocalGameWorld/CLocalGameWorld.h"
#include "Classes/CRegisteredPlayers/CRegisteredPlayers.h"
#include "Classes/CLootList/CLootList.h"
#include "Classes/CExfilController/CExfilController.h"

class EFT
{
public:
	static bool Initialize(DMA_Connection* Conn);
	static const Process& GetProcess();

private:
	static inline Process Proc{};
public:
	static void MakeNewGameWorld(DMA_Connection* Conn);

public:
	static void QuickUpdatePlayers(DMA_Connection* Conn);
	static void HandlePlayerAllocations(DMA_Connection* Conn);

public:
	static inline std::unique_ptr<class CLocalGameWorld> pGameWorld{ nullptr };
	static class CRegisteredPlayers& GetRegisteredPlayers();
	static class CLootList& GetLootList();
	static class CExfilController& GetExfilController();
};