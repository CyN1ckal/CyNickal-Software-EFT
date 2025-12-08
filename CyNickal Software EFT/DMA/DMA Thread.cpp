#include "pch.h"
#include "DMA Thread.h"
#include "Game/EFT.h"
#include "Game/Player List/Player List.h"
#include "Game/Loot List/Loot List.h"
#include "Game/GOM/GOM.h"
#include "Game/Camera/Camera.h"

extern std::atomic<bool> bRunning;

void DMA_Thread_Main()
{
	std::println("[DMA Thread] DMA Thread started.");

	DMA_Connection* Conn = DMA_Connection::GetInstance();

	EFT::Initialize(Conn);

	auto LocalGameWorldAddr = GOM::GetLocalGameWorldAddr(Conn);

	CTimer Player_Quick(std::chrono::milliseconds(25), [&Conn]() { PlayerList::QuickUpdate(Conn); });
	CTimer Player_Complete(std::chrono::seconds(5), [&Conn, LocalGameWorldAddr]() { PlayerList::CompleteUpdate(Conn, LocalGameWorldAddr); });
	CTimer Loot_Quick(std::chrono::milliseconds(500), [&Conn]() { LootList::QuickUpdate(Conn); });
	CTimer Camera_UpdateViewMatrix(std::chrono::milliseconds(2), [&Conn]() { Camera::QuickUpdateViewMatrix(Conn); });

	while (bRunning)
	{
		auto TimeNow = std::chrono::high_resolution_clock::now();
		Player_Quick.Tick(TimeNow);
		Player_Complete.Tick(TimeNow);
		Loot_Quick.Tick(TimeNow);
		Camera_UpdateViewMatrix.Tick(TimeNow);
	}

	Conn->EndConnection();
}