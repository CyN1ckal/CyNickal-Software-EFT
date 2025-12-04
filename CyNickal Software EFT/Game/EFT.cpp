#include "pch.h"
#include "EFT.h"
#include "Game/GOM/GOM.h"

bool EFT::Initialize(DMA_Connection* Conn)
{
	std::println("Initializing EFT module...");

	Proc.GetProcessInfo(Conn);

	GOM::Initialize(Conn);

	return false;
}

const Process& EFT::GetProcess()
{
	return Proc;
}