#include "pch.h"
#include "EFT.h"
#include "Game/GOM/GOM.h"
#include "Game/Camera List/Camera List.h"
#include "Game/Response Data/Response Data.h"

bool EFT::Initialize(DMA_Connection* Conn)
{
	std::println("Initializing EFT module...");

	Proc.GetProcessInfo(Conn);

	MakeNewGameWorld(Conn);

	ResponseData::Initialize(Conn);

	return true;
}

const Process& EFT::GetProcess()
{
	return Proc;
}

void EFT::MakeNewGameWorld(DMA_Connection* Conn)
{
	try
	{
		GOM::Initialize(Conn);
		pGameWorld = std::make_unique<CLocalGameWorld>(GOM::FindGameWorldAddressFromCache(Conn));
		CameraList::Initialize(Conn);
	}
	catch (const std::exception& e)
	{
		std::println("[EFT] Failed making new game world! {}", e.what());
	}
}

void EFT::QuickUpdatePlayers(DMA_Connection* Conn)
{
	if (pGameWorld)
		pGameWorld->QuickUpdatePlayers(Conn);
}

void EFT::HandlePlayerAllocations(DMA_Connection* Conn)
{
	if (pGameWorld)
		pGameWorld->HandlePlayerAllocations(Conn);
}

CRegisteredPlayers& EFT::GetRegisteredPlayers()
{
	if (!pGameWorld)
		throw std::runtime_error("EFT::pGameWorld is null");

	if (!pGameWorld->m_pRegisteredPlayers)
		throw std::runtime_error("EFT::pGameWorld->m_pRegisteredPlayers is null");

	return *(pGameWorld->m_pRegisteredPlayers);
}

CLootList& EFT::GetLootList()
{
	if (!pGameWorld)
		throw std::runtime_error("EFT::pGameWorld is null");

	if (!pGameWorld->m_pLootList)
		throw std::runtime_error("EFT::pGameWorld->m_pLootList is null");

	return *(pGameWorld->m_pLootList);
}

CExfilController& EFT::GetExfilController()
{
	if (!pGameWorld)
		throw std::runtime_error("EFT::pGameWorld is null");

	if (!pGameWorld->m_pExfilController)
		throw std::runtime_error("EFT::pGameWorld->m_pRegisteredExfils is null");

	return *(pGameWorld->m_pExfilController);
}
