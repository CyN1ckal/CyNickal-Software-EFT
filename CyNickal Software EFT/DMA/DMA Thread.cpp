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

void CheckRaidStatus(DMA_Connection* Conn)
{
	static bool bNeedFullScan = false;
	static int QuickScanFailCount = 0;
	static uintptr_t CachedLocalGameWorld = 0;

	auto& Proc = EFT::GetProcess();

	try
	{
		if (bInRaid && CachedLocalGameWorld != 0)
		{
			// We're in raid - just validate the pointer without scanning
			// Read multiple fields to ensure it's still valid
			uintptr_t MainPlayerAddr = Proc.ReadMem<uintptr_t>(Conn, CachedLocalGameWorld + Offsets::CLocalGameWorld::pMainPlayer);

			// Basic sanity check
			if (MainPlayerAddr == 0 || MainPlayerAddr > 0x7FFFFFFFFFFF)
			{
				std::println("[DMA Thread] MainPlayer pointer invalid - left raid!");
				bInRaid = false;
				CachedLocalGameWorld = 0;
				bNeedFullScan = false;
				return;
			}

			// Additional validation - try reading from MainPlayer
			// If this throws or returns garbage, we've left raid
			uintptr_t ProfileAddr = Proc.ReadMem<uintptr_t>(Conn, MainPlayerAddr + 0x10); // Adjust offset as needed
			if (ProfileAddr == 0 || ProfileAddr > 0x7FFFFFFFFFFF)
			{
				std::println("[DMA Thread] MainPlayer data invalid - left raid!");
				bInRaid = false;
				CachedLocalGameWorld = 0;
				bNeedFullScan = false;
				return;
			}

			// Still valid - we're in raid, exit early
			return;
		}

		// Not in raid - scan for LocalGameWorld
		// Always update GOM pointers
		uintptr_t pGOMAddress = Proc.GetUnityAddress() + Offsets::pGOM;
		GOM::GameObjectManagerAddress = Proc.ReadMem<uintptr_t>(Conn, pGOMAddress);
		GOM::LastActiveNode = Proc.ReadMem<uintptr_t>(Conn, GOM::GameObjectManagerAddress + Offsets::CGameObjectManager::pLastActiveNode);
		GOM::ActiveNodes = Proc.ReadMem<uintptr_t>(Conn, GOM::GameObjectManagerAddress + Offsets::CGameObjectManager::pActiveNodes);

		// Decide scan type
		if (bNeedFullScan)
		{
			std::println("[DMA Thread] Doing full scan...");
			GOM::GetObjectAddresses(Conn, UINT32_MAX);
			bNeedFullScan = false;
			QuickScanFailCount = 0;
		}
		else
		{
			std::println("[DMA Thread] Doing quick scan...");
			GOM::GetObjectAddresses(Conn, 10000);
		}

		GOM::PopulateObjectInfoListFromAddresses(Conn);

		// Try to find LocalGameWorld
		CachedLocalGameWorld = GOM::FindGameWorldAddressFromCache(Conn);

		// Found it!
		QuickScanFailCount = 0;

		std::println("[DMA Thread] Entered raid! Initializing GameWorld...");
		EFT::MakeNewGameWorld(Conn);
		bInRaid = true;
	}
	catch (const std::exception& e)
	{
		QuickScanFailCount++;

		// If quick scans fail multiple times, do a full scan
		if (QuickScanFailCount >= 2 && !bNeedFullScan)
		{
			std::println("[DMA Thread] Quick scans failing, will do full scan next time");
			bNeedFullScan = true;
		}

		if (bInRaid)
		{
			std::println("[DMA Thread] Left raid (exception)!");
			bInRaid = false;
			CachedLocalGameWorld = 0;
			bNeedFullScan = false;
		}
	}
}

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
	CTimer RaidCheck(std::chrono::seconds(10), [&Conn]() { CheckRaidStatus(Conn); });
	CTimer ResponseData(std::chrono::milliseconds(25), [&Conn]() {
		if (bInRaid) ResponseData::OnDMAFrame(Conn);
		});
	CTimer Player_Quick(std::chrono::milliseconds(25), [&Conn]() {
		if (bInRaid) EFT::QuickUpdatePlayers(Conn);
		});
	CTimer Player_Allocations(std::chrono::seconds(5), [&Conn]() {
		if (bInRaid) EFT::HandlePlayerAllocations(Conn);
		});
	CTimer Camera_UpdateViewMatrix(std::chrono::milliseconds(2), [&Conn]() {
		if (bInRaid) CameraList::QuickUpdateNecessaryCameras(Conn);
		});
	CTimer Keybinds(std::chrono::milliseconds(50), [&Conn]() { Keybinds::OnDMAFrame(Conn); });

	while (bRunning)
	{
		auto TimeNow = std::chrono::high_resolution_clock::now();
		LightRefresh.Tick(TimeNow);
		RaidCheck.Tick(TimeNow);

		// Only tick these if in raid
		if (bInRaid)
		{
			ResponseData.Tick(TimeNow);
			Player_Quick.Tick(TimeNow);
			Player_Allocations.Tick(TimeNow);
			Camera_UpdateViewMatrix.Tick(TimeNow);
		}

		Keybinds.Tick(TimeNow); // Always check keybinds
	}

	Conn->EndConnection();
}