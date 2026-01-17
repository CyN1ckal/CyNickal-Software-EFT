#include "pch.h"
#include "CLocalGameWorld.h"
#include "Game/Offsets/Offsets.h"
#include "DMA/DMA.h"
#include "Game/EFT.h"

CLocalGameWorld::CLocalGameWorld(uintptr_t GameWorldAddress) : CBaseEntity(GameWorldAddress)
{
	std::println("[CLocalGameWorld] Created CLocalGameWorld at address: 0x{:X}", GameWorldAddress);

	auto Conn = DMA_Connection::GetInstance();
	auto& Proc = EFT::GetProcess();

	LootListAddress = Proc.ReadMem<uintptr_t>(Conn, GameWorldAddress + Offsets::CLocalGameWorld::pLootList);
	m_pLootList = std::make_unique<CLootList>(LootListAddress);

	RegisteredPlayersAddress = Proc.ReadMem<uintptr_t>(Conn, GameWorldAddress + Offsets::CLocalGameWorld::pRegisteredPlayers);
	m_pRegisteredPlayers = std::make_unique<CRegisteredPlayers>(RegisteredPlayersAddress);

	ExfiltrationControllerAddress = Proc.ReadMem<uintptr_t>(Conn, GameWorldAddress + Offsets::CLocalGameWorld::pExfiltrationController);
	m_pExfilController = std::make_unique<CExfilController>(ExfiltrationControllerAddress);

	m_MainPlayerAddress = Proc.ReadMem<uintptr_t>(Conn, GameWorldAddress + Offsets::CLocalGameWorld::pMainPlayer);
}

void CLocalGameWorld::QuickUpdatePlayers(DMA_Connection* Conn)
{
	if (m_pRegisteredPlayers)
		m_pRegisteredPlayers->QuickUpdate(Conn);
}

void CLocalGameWorld::HandlePlayerAllocations(DMA_Connection* Conn)
{
	if (m_pRegisteredPlayers == nullptr) return;

	m_pRegisteredPlayers->UpdateBaseAddresses(Conn);
	m_pRegisteredPlayers->HandlePlayerAllocations(Conn);
}

bool CLocalGameWorld::IsValidRaid(DMA_Connection* Conn)
{
	if (IsInvalid()) return false;

	if (m_pExfilController == nullptr || m_pExfilController->IsInvalid()) return false;

	if (m_pRegisteredPlayers == nullptr || m_pRegisteredPlayers->IsInvalid()) return false;

	if (m_pLootList == nullptr || m_pLootList->IsInvalid()) return false;

	if (m_pRegisteredPlayers->GetNumValidPlayers() == 0) return false;

	return true;
}