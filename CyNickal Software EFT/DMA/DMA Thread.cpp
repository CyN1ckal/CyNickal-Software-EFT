#include "pch.h"
#include "DMA Thread.h"
#include "Input Manager.h"
#include "Game/EFT.h"
#include "Game/Response Data/Response Data.h"

#include "Game/GOM/GOM.h"
#include "Game/Camera List/Camera List.h"
#include "GUI/Aimbot/Aimbot.h"
#include "GUI/Keybinds/Keybinds.h"
#include "Game/Offsets/Offsets.h"

extern std::atomic<bool> bRunning;
static std::atomic<bool> bInRaid = false;

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
	CTimer RaidCheck(std::chrono::seconds(10), [&Conn]() {
		EFT::EnsureInRaid(Conn);
		});

	CTimer ResponseData(std::chrono::milliseconds(25), [&Conn]() {
		if (EFT::pGameWorld) ResponseData::OnDMAFrame(Conn);
		});
	CTimer Player_Quick(std::chrono::milliseconds(25), [&Conn]() {
		if (EFT::pGameWorld) EFT::QuickUpdatePlayers(Conn);
		});
	CTimer Player_Allocations(std::chrono::seconds(5), [&Conn]() {
		if (EFT::pGameWorld) EFT::HandlePlayerAllocations(Conn);
		});
	CTimer Camera_UpdateViewMatrix(std::chrono::milliseconds(2), [&Conn]() {
		if (EFT::pGameWorld) CameraList::QuickUpdateNecessaryCameras(Conn);
		});
	CTimer Keybinds(std::chrono::milliseconds(50), [&Conn]() { Keybinds::OnDMAFrame(Conn); });

	while (bRunning)
	{
		auto TimeNow = std::chrono::high_resolution_clock::now();
		LightRefresh.Tick(TimeNow);
		RaidCheck.Tick(TimeNow);
		ResponseData.Tick(TimeNow);
		Player_Quick.Tick(TimeNow);
		Player_Allocations.Tick(TimeNow);
		Camera_UpdateViewMatrix.Tick(TimeNow);
		Keybinds.Tick(TimeNow);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	Conn->EndConnection();
}