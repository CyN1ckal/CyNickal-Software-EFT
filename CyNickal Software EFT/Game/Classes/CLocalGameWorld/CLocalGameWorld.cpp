#include "pch.h"
#include "CLocalGameWorld.h"
#include "Game/Offsets/Offsets.h"
#include "DMA/DMA.h"
#include "Game/EFT.h"
#include "Game/GOM/GOM.h"

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
}

void CLocalGameWorld::QuickUpdatePlayers(DMA_Connection* Conn)
{
	if(m_pRegisteredPlayers)
		m_pRegisteredPlayers->QuickUpdate(Conn);
}

void CLocalGameWorld::HandlePlayerAllocations(DMA_Connection* Conn)
{
	if (m_pRegisteredPlayers == nullptr) return;

	m_pRegisteredPlayers->UpdateBaseAddresses(Conn);
	m_pRegisteredPlayers->HandlePlayerAllocations(Conn);
}

bool CLocalGameWorld::Validate(DMA_Connection* Conn, std::unique_ptr<CLocalGameWorld>& pGameWorld)
{
    try
    {
        auto& Proc = EFT::GetProcess();
        uintptr_t GameWorldAddr = pGameWorld->GetAddress();

        // Read MainPlayer pointer
        uintptr_t MainPlayerAddr = Proc.ReadMem<uintptr_t>(Conn, GameWorldAddr + Offsets::CLocalGameWorld::pMainPlayer);

        // Check if MainPlayer pointer is valid
        if (MainPlayerAddr == 0 || MainPlayerAddr > 0x7FFFFFFFFFFF)
        {
            std::println("[CLocalGameWorld] MainPlayer pointer invalid - left raid!");
            pGameWorld.reset();
            return false;
        }

        // Try to read from MainPlayer to verify it's accessible
        // Read Profile pointer (adjust offset as needed - typically around 0x10-0x18)
        uintptr_t ProfileAddr = Proc.ReadMem<uintptr_t>(Conn, MainPlayerAddr + 0x10);

        if (ProfileAddr == 0 || ProfileAddr > 0x7FFFFFFFFFFF)
        {
            std::println("[CLocalGameWorld] MainPlayer Profile invalid - left raid!");
            pGameWorld.reset();
            return false;
        }

        // All checks passed - still in raid
        return true;
    }
    catch (...)
    {
        // Any exception means we can't read the data - left raid
        std::println("[CLocalGameWorld] Failed to read MainPlayer data - left raid!");
        pGameWorld.reset();
        return false;
    }
}

void CLocalGameWorld::EnsureGameWorld(DMA_Connection* Conn, std::unique_ptr<CLocalGameWorld>& pGameWorld)
{
    static int FailCount = 0;

    // Validate existing GameWorld if it exists
    if (pGameWorld != nullptr)
    {
        if (Validate(Conn, pGameWorld))
        {
            // Still valid
            FailCount = 0;
            return;
        }
        // Validation failed and nulled pGameWorld - fall through to search
    }

    // No GameWorld - search for one
    try
    {
        auto& Proc = EFT::GetProcess();

        // Update GOM pointers
        uintptr_t pGOMAddress = Proc.GetUnityAddress() + Offsets::pGOM;
        GOM::GameObjectManagerAddress = Proc.ReadMem<uintptr_t>(Conn, pGOMAddress);
        GOM::LastActiveNode = Proc.ReadMem<uintptr_t>(Conn, GOM::GameObjectManagerAddress + Offsets::CGameObjectManager::pLastActiveNode);
        GOM::ActiveNodes = Proc.ReadMem<uintptr_t>(Conn, GOM::GameObjectManagerAddress + Offsets::CGameObjectManager::pActiveNodes);

        // Escalate to full scan after every 3rd failure
        if (FailCount > 0 && FailCount % 3 == 0)
        {
            std::println("[CLocalGameWorld] Doing full scan (fail count: {})...", FailCount);
            GOM::GetObjectAddresses(Conn, UINT32_MAX);
        }
        else
        {
            std::println("[CLocalGameWorld] Doing quick scan...");
            GOM::GetObjectAddresses(Conn, 10000);
        }

        GOM::PopulateObjectInfoListFromAddresses(Conn);

        // Try to find and create LocalGameWorld
        //std::println("[CLocalGameWorld] Found LocalGameWorld! Initializing...");
        EFT::MakeNewGameWorld(Conn);

        FailCount = 0;
        //std::println("[CLocalGameWorld] Successfully entered raid!");
    }
    catch (const std::exception& e)
    {
        FailCount++;
    }
}