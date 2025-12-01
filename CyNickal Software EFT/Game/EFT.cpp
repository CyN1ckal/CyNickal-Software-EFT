#include "pch.h"
#include "EFT.h"
#include "Classes/CGameObjectManager.h"
#include "Classes/CLinkedListEntry.h"

bool EFT::Initialize(DMA_Connection* Conn)
{
	std::println("Initializing EFT module...");

	Proc.GetProcessInfo(Conn);

	uintptr_t pGOMAddress = Proc.GetUnityAddress() + 0x1A208D8;
	GameObjectManagerAddress = Proc.ReadMem<uintptr_t>(Conn, pGOMAddress);
	std::println("GameObjectManager Address: 0x{:X}", GameObjectManagerAddress);

	LastActiveNode = Proc.ReadMem<uintptr_t>(Conn, GameObjectManagerAddress + offsetof(CGameObjectManager, pLastActiveNode));
	std::println("LastActiveNode Address: 0x{:X}", LastActiveNode);

	ActiveNodes = Proc.ReadMem<uintptr_t>(Conn, GameObjectManagerAddress + offsetof(CGameObjectManager, pActiveNodes));
	std::println("ActiveNodes Address: 0x{:X}", ActiveNodes);

	auto StartTime = std::chrono::high_resolution_clock::now();

	uint32_t NodeCount = 0;
	uintptr_t CurrentActiveNode = ActiveNodes;
	while (CurrentActiveNode != LastActiveNode)
	{
		CLinkedListEntry NodeEntry = Proc.ReadMem<CLinkedListEntry>(Conn, CurrentActiveNode);
		NodeEntry.Print();
		CurrentActiveNode = NodeEntry.pNextObject;
		NodeCount++;
	}

	auto EndTime = std::chrono::high_resolution_clock::now();

	auto Duration = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime).count();
	std::println("Finished iterating {} active nodes in {} ms", NodeCount, Duration);

	return false;
}

const Process& EFT::GetProcess()
{
	return Proc;
}