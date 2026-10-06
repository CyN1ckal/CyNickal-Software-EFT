/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "DMA Thread.h"
#include "DMA/CInputManager/CInputManager.h"
#include "Game/EFT.h"
#include "Game/Response Data/Response Data.h"

#include "Game/Camera List/Camera List.h"
#include "GUI/Windows/Keybinds/Keybinds.h"
#include "Game/Quest Manager/Quest Manager.h"
#include "Game/Exploits/Exploits.h"

extern std::atomic<bool> bRunning;

void DMA_Thread_Main()
{
	tracy::SetThreadName("DMA Thread");

	std::println("[DMA Thread] DMA Thread started.");

	CDMAConnection* Conn = CDMAConnection::GetInstance();

	CInputManager::InitKeyboard(Conn);

	if (!EFT::Initialize(Conn))
	{
		std::println("[DMA Thread] EFT Initialization failed, requesting exit.");
		bRunning = false;
		return;
	}

	CTimer LightRefresh(std::chrono::seconds(5), [&Conn]() { Conn->LightRefresh(); });
	
	CTimer Camera_UpdateViewMatrix(std::chrono::milliseconds(1), [&Conn]() { CameraList::QuickUpdateNecessaryCameras(Conn); });
	CTimer RaidCheck(std::chrono::seconds(10), [&Conn]() { EFT::CreateWorldIfNeeded(Conn);	});
	CTimer Keybinds(std::chrono::milliseconds(50), [&Conn]() { Keybinds::OnDMAFrame(Conn); });
	CTimer Exploits(std::chrono::milliseconds(200), [&Conn]() { Exploits::OnDMAFrame(Conn); });
	CTimer ResponseData(std::chrono::milliseconds(25), [&Conn]() { ResponseData::OnDMAFrame(Conn); });

	CTimer Item_Quick(std::chrono::milliseconds(500), [&Conn]() { EFT::QuickUpdateItems(Conn); });

	CTimer Player_Quick(std::chrono::milliseconds(2), [&Conn]() { EFT::QuickUpdatePlayers(Conn); });
	CTimer Player_Allocations(std::chrono::seconds(5), [&Conn]() { EFT::HandlePlayerAllocations(Conn); });

	CTimer Grenades_Quick(std::chrono::milliseconds(11), [&Conn]() { EFT::QuickUpdateGrenades(Conn); });
	CTimer Grenades_Full(std::chrono::milliseconds(200), [&Conn]() { EFT::FullUpdateGrenades(Conn); });

	while (bRunning)
	{
		auto TimeNow = std::chrono::high_resolution_clock::now();
		LightRefresh.Tick(TimeNow);
		RaidCheck.Tick(TimeNow);
		ResponseData.Tick(TimeNow);
		Item_Quick.Tick(TimeNow);
		Player_Quick.Tick(TimeNow);
		Player_Allocations.Tick(TimeNow);
		Camera_UpdateViewMatrix.Tick(TimeNow);
		Keybinds.Tick(TimeNow);
		Exploits.Tick(TimeNow);
		Grenades_Full.Tick(TimeNow);
		Grenades_Quick.Tick(TimeNow);
	}

	Conn->EndConnection();
}