#include "pch.h"
#include "DMA Thread.h"
#include "Input Manager.h"
#include "Game/EFT.h"
#include "Game/Player List/Player List.h"
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

	EFT::Initialize(Conn);

	auto LocalGameWorldAddr = GOM::GetLocalGameWorldAddr(Conn);

	CTimer Player_Quick(std::chrono::milliseconds(25), [&Conn]() { PlayerList::QuickUpdate(Conn); });
	CTimer Player_Allocations(std::chrono::seconds(5), [&Conn, &LocalGameWorldAddr]() {
		PlayerList::UpdateBaseAddresses(Conn, LocalGameWorldAddr);
		PlayerList::HandlePlayerAllocations(Conn);
		});
	CTimer Camera_UpdateViewMatrix(std::chrono::milliseconds(2), [&Conn]() { Camera::QuickUpdateViewMatrix(Conn); });
	CTimer LightRefresh(std::chrono::seconds(5), [&Conn]() { Conn->LightRefresh(); });
	CTimer Keybinds(std::chrono::milliseconds(50), [&Conn]() { Keybinds::OnDMAFrame(Conn); });

	while (bRunning)
	{
		auto TimeNow = std::chrono::high_resolution_clock::now();
		LightRefresh.Tick(TimeNow);
		Player_Quick.Tick(TimeNow);
		Player_Allocations.Tick(TimeNow);
		Camera_UpdateViewMatrix.Tick(TimeNow);
		Keybinds.Tick(TimeNow);
	}

	Conn->EndConnection();
}