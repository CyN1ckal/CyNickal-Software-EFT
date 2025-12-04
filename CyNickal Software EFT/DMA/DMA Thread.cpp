#include "pch.h"
#include "DMA Thread.h"
#include "Game/EFT.h"
#include "Game/Player List/Player List.h"
#include "Game/GOM/GOM.h"

extern std::atomic<bool> bRunning;

void DMA_Thread_Main()
{
	std::println("[DMA Thread] DMA Thread started.");

	DMA_Connection* Conn = DMA_Connection::GetInstance();

	EFT::Initialize(Conn);

	auto LocalGameWorldAddr = GOM::GetLocalGameWorldAddr(Conn);

	PlayerList::CompleteUpdate(Conn, LocalGameWorldAddr);

	CTimer Player_Quick(std::chrono::milliseconds(100), [&Conn]() { PlayerList::QuickUpdate(Conn); });
	CTimer Player_Complete(std::chrono::seconds(5), [&Conn, LocalGameWorldAddr]() { PlayerList::CompleteUpdate(Conn, LocalGameWorldAddr); });

	while (bRunning)
	{
		auto TimeNow = std::chrono::high_resolution_clock::now();
		Player_Quick.Tick(TimeNow);
		Player_Complete.Tick(TimeNow);
	}

	Conn->EndConnection();
}