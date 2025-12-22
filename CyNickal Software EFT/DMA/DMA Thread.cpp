#include "pch.h"
#include "DMA Thread.h"
#include "Input Manager.h"
#include "Game/EFT.h"
#include "Game/Response Data/Response Data.h"

#include "Game/GOM/GOM.h"
#include "Game/Camera/Camera.h"
#include "GUI/Aimbot/Aimbot.h"
#include "GUI/Keybinds/Keybinds.h"

extern std::atomic<bool> bRunning;

void DMA_Thread_Main()
{
	std::println("[DMA Thread] DMA Thread started.");

	DMA_Connection* Conn = DMA_Connection::GetInstance();

	c_keys::InitKeyboard(Conn);

	if (!EFT::Initialize(Conn))
	{
		std::println("[DMA Thread] EFT Initialization failed, requesting exit.");
		bRunning = false;
		return;
	}

	CTimer LightRefresh(std::chrono::seconds(5), [&Conn]() { Conn->LightRefresh(); });
	CTimer ResponseData(std::chrono::milliseconds(25), [&Conn]() { ResponseData::OnDMAFrame(Conn); });
	CTimer Player_Quick(std::chrono::milliseconds(25), [&Conn]() { EFT::QuickUpdatePlayers(Conn); });
	CTimer Player_Allocations(std::chrono::seconds(5), [&Conn]() { EFT::HandlePlayerAllocations(Conn); });
	CTimer Camera_UpdateViewMatrix(std::chrono::milliseconds(2), [&Conn]() { Camera::QuickUpdateViewMatrix(Conn); });
	CTimer Keybinds(std::chrono::milliseconds(50), [&Conn]() { Keybinds::OnDMAFrame(Conn); });

	while (bRunning)
	{
		auto TimeNow = std::chrono::high_resolution_clock::now();
		LightRefresh.Tick(TimeNow);
		ResponseData.Tick(TimeNow);
		Player_Quick.Tick(TimeNow);
		Player_Allocations.Tick(TimeNow);
		Camera_UpdateViewMatrix.Tick(TimeNow);
		Keybinds.Tick(TimeNow);
	}

	Conn->EndConnection();
}