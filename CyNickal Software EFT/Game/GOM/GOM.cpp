/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "GOM.h"
#include "Game/EFT.h"
#include <fstream>
#include <unordered_set>
#include "Game/Offsets/Offsets.h"
#include "Game/Classes/CLinkedListEntry.h"
#include "GUI/Windows/Flea Bot/Flea Bot.h"

bool GOM::Initialize(CDMAConnection* Conn)
{
	ZoneScoped;

	std::println("[GOM] Initializing...");

	auto& Proc = EFT::GetProcess();

	uintptr_t pGOMAddress = Proc.GetUnityAddress() + Offsets::pGOM;
	GameObjectManagerAddress = Proc.ReadMem<uintptr_t>(Conn, pGOMAddress);

	LastActiveNode = Proc.ReadMem<uintptr_t>(Conn, GameObjectManagerAddress + Offsets::CGameObjectManager::pLastActiveNode);

	ActiveNodes = Proc.ReadMem<uintptr_t>(Conn, GameObjectManagerAddress + Offsets::CGameObjectManager::pActiveNodes);

	GetObjectAddresses(Conn, 50000);

	PopulateObjectInfoListFromAddresses(Conn);

	SaveAllObjectsToFile("GOM_ObjectList.txt");

	std::println("[GOM] Finished.");

	return false;
}

void GOM::GetObjectAddresses(CDMAConnection* Conn, uint32_t MaxNodes)
{
	auto& Proc = EFT::GetProcess();

	m_ObjectAddresses.clear();

	auto StartTime = std::chrono::high_resolution_clock::now();
	uint32_t NodeCount = 0;
	DWORD BytesRead = 0;
	uintptr_t CurrentActiveNode = ActiveNodes;
	uintptr_t FirstNode = ActiveNodes;

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), EFT::GetProcess().GetPID(), 0);

	// Track visited nodes to avoid duplicates when doing bidirectional traversal
	std::unordered_set<uintptr_t> VisitedNodes;

	// Forward traversal
	uint32_t ForwardCount = 0;
	while (true)
	{
		if (CurrentActiveNode == LastActiveNode && ForwardCount > 5)
			break;

		if (ForwardCount >= MaxNodes)
			break;

		if (VisitedNodes.contains(CurrentActiveNode))
			break;

		CLinkedListEntry NodeEntry{};
		VMMDLL_Scatter_PrepareEx(vmsh, CurrentActiveNode, sizeof(CLinkedListEntry), reinterpret_cast<BYTE*>(&NodeEntry), &BytesRead);
		VMMDLL_Scatter_Execute(vmsh);
		VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), 0);

		if (BytesRead != sizeof(CLinkedListEntry))
			break;

		VisitedNodes.insert(CurrentActiveNode);
		m_ObjectAddresses.push_back(NodeEntry.pObject);

		if (NodeEntry.pNextEntry == FirstNode)
			break;

		if (NodeEntry.pNextEntry == 0)
			break;

		CurrentActiveNode = NodeEntry.pNextEntry;
		ForwardCount++;
	}

	// Backward traversal - start from ActiveNodes and go backwards
	uint32_t BackwardCount = 0;
	CurrentActiveNode = ActiveNodes;

	// First, read the ActiveNodes entry to get its pPreviousEntry
	CLinkedListEntry StartEntry{};
	VMMDLL_Scatter_PrepareEx(vmsh, CurrentActiveNode, sizeof(CLinkedListEntry), reinterpret_cast<BYTE*>(&StartEntry), &BytesRead);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), 0);

	if (BytesRead == sizeof(CLinkedListEntry) && StartEntry.pPreviousEntry != 0)
	{
		CurrentActiveNode = StartEntry.pPreviousEntry;

		while (true)
		{
			if (BackwardCount >= MaxNodes)
				break;

			if (VisitedNodes.contains(CurrentActiveNode))
				break;

			if (CurrentActiveNode == LastActiveNode && BackwardCount > 5)
				break;

			CLinkedListEntry NodeEntry{};
			VMMDLL_Scatter_PrepareEx(vmsh, CurrentActiveNode, sizeof(CLinkedListEntry), reinterpret_cast<BYTE*>(&NodeEntry), &BytesRead);
			VMMDLL_Scatter_Execute(vmsh);
			VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), 0);

			if (BytesRead != sizeof(CLinkedListEntry))
				break;

			VisitedNodes.insert(CurrentActiveNode);
			m_ObjectAddresses.push_back(NodeEntry.pObject);

			if (NodeEntry.pPreviousEntry == FirstNode)
				break;

			if (NodeEntry.pPreviousEntry == 0)
				break;

			CurrentActiveNode = NodeEntry.pPreviousEntry;
			BackwardCount++;
		}
	}

	NodeCount = ForwardCount + BackwardCount;
	VMMDLL_Scatter_CloseHandle(vmsh);

	auto EndTime = std::chrono::high_resolution_clock::now();
	auto Duration = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime).count();
	std::println("[GOM] UpdateObjectList; {} total nodes in {}ms", NodeCount, Duration);
}

std::vector<uintptr_t> GOM::GetPotentialGameWorlds()
{
	std::vector<uintptr_t> GameWorldAddresses{};

	for (auto& ObjInfo : m_ObjectInfo)
	{
		if (ObjInfo.m_ObjectName == "GameWorld") {
			GameWorldAddresses.push_back(ObjInfo.m_EntityAddress);
		}
	}

	return GameWorldAddresses;
}

uintptr_t GOM::GetWinningGameWorld(CDMAConnection* Conn)
{
	auto& Proc = EFT::GetProcess();

	std::println("[GOM] Searching for valid GameWorld in {} objects...", m_ObjectInfo.size());

	for (auto& ObjInfo : m_ObjectInfo) {
		if (ObjInfo.IsInvalid()) continue;

		if (ObjInfo.m_ObjectName.at(0) == 'G' && ObjInfo.m_ObjectName.at(4) == 'W') {
			auto LocalWorldAddr = ObjInfo.m_Components[0].GetComponentClassAddress();
			auto MainPlayerAddr = Proc.ReadMem<uintptr_t>(Conn, LocalWorldAddr + Offsets::CLocalGameWorld::pMainPlayer);

			if (MainPlayerAddr) {
				std::println("[EFT] LocalGameWorld found @ 0x{:X}\n", LocalWorldAddr);
				return LocalWorldAddr;
			}
		}
	}

	throw std::runtime_error("Failed to find valid LocalGameWorld address.");
}

void GOM::PopulateObjectInfoListFromAddresses(CDMAConnection* Conn)
{
	ZoneScoped;

	m_ObjectInfo.clear();

	m_ObjectInfo = CGameObject::ScatterFactory(m_ObjectAddresses);
}

uintptr_t GOM::GetLatestWorldAddr(CDMAConnection* Conn)
{
	GOM::Initialize(Conn);

	uintptr_t Return{};

	try {
		Return = GetWinningGameWorld(Conn);
	}
	catch (const std::exception& e)
	{
		std::println("[EFT] GetLatestWorldAddr; Exception: {}", e.what());
	}

	return Return;
}

void GOM::SaveAllObjectsToFile(const std::string& FileName)
{
	std::ofstream OutFile(FileName, std::ios::out | std::ios::trunc);

	for (auto& ObjInfo : m_ObjectInfo) {
		if (ObjInfo.IsInvalid()) continue;

		OutFile << std::format("Entity @ {0:X} named `{1:s}`", ObjInfo.m_EntityAddress, ObjInfo.m_ObjectName.c_str()) << std::endl;

		for (auto& Component : ObjInfo.m_Components) {
			if (Component.IsInvalid()) continue;

			OutFile << std::format("   Component @ {0:X} named `{1:s}`", Component.m_EntityAddress, Component.m_ComponentName.c_str()) << std::endl;
		}
	}

	OutFile.close();
}

uintptr_t GOM::FindComponentInGOM(std::string ComponentName)
{
	for (auto& Obj : m_ObjectInfo) {
		if (Obj.IsInvalid()) continue;

		for (auto& Component : Obj.m_Components) {
			if (Component.IsInvalid()) continue;

			if (Component.m_ComponentName.contains(ComponentName))
				return Component.GetComponentClassAddress();
		}
	}

	return uintptr_t();
}