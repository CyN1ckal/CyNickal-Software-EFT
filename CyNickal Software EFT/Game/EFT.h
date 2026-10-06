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
#include "DMA/CProcess/CProcess.h"
#include "Classes/CLocalGameWorld/CLocalGameWorld.h"
#include "Classes/CRegisteredPlayers/CRegisteredPlayers.h"
#include "Classes/CLootList/CLootList.h"
#include "Classes/CExfilController/CExfilController.h"

class EFT
{
public:
	static bool Initialize(CDMAConnection* Conn);
	static const CProcess& GetProcess();

private:
	static inline CProcess Proc{};

public:
	static void CreateWorldIfNeeded(CDMAConnection* Conn);
	static uintptr_t GetMainPlayerAddress();

public:
	static void QuickUpdateItems(CDMAConnection* Conn);
	static void QuickUpdatePlayers(CDMAConnection* Conn);
	static void QuickUpdateGrenades(CDMAConnection* Conn);
	static void HandlePlayerAllocations(CDMAConnection* Conn);
	static void HandleLootListAllocations(CDMAConnection* Conn);
	static void FullUpdateGrenades(CDMAConnection* Conn);

public:
	static inline std::mutex m_GameWorldMutex{};
	static inline std::unique_ptr<class CLocalGameWorld> pGameWorld{ nullptr };

	static class CRegisteredPlayers& GetRegisteredPlayers();
	static class CLootList& GetLootList();
	static class CExfilController& GetExfilController();

public:
	static EMap GetCurrentMap();
};