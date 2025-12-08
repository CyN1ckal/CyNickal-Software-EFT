#include "pch.h"
#include "EFT.h"
#include "Game/GOM/GOM.h"
#include "Game/Camera/Camera.h"
#include "Game/Player List/Player List.h"
#include "Game/Loot List/Loot List.h"

bool EFT::Initialize(DMA_Connection* Conn)
{
	std::println("Initializing EFT module...");

	Proc.GetProcessInfo(Conn);

	GOM::Initialize(Conn);

	Camera::Initialize(Conn);

	auto LocalGameWorldAddr = GOM::GetLocalGameWorldAddr(Conn);

	LootList::CompleteUpdate(Conn, LocalGameWorldAddr);

	return false;
}

const Process& EFT::GetProcess()
{
	return Proc;
}