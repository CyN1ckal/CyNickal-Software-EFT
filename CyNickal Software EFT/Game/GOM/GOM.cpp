#include "pch.h"
#include "GOM.h"
#include "Game/EFT.h"
#include <fstream>
#include <unordered_set>
#include "Game/Offsets/Offsets.h"
#include "Game/Classes/CLinkedListEntry.h"

bool GOM::Initialize(DMA_Connection* Conn)
{
	auto& Proc = EFT::GetProcess();

	uintptr_t pGOMAddress = Proc.GetUnityAddress() + Offsets::pGOM;
	GameObjectManagerAddress = Proc.ReadMem<uintptr_t>(Conn, pGOMAddress);

	LastActiveNode = Proc.ReadMem<uintptr_t>(Conn, GameObjectManagerAddress + Offsets::CGameObjectManager::pLastActiveNode);

	ActiveNodes = Proc.ReadMem<uintptr_t>(Conn, GameObjectManagerAddress + Offsets::CGameObjectManager::pActiveNodes);

	GetObjectAddresses(Conn, 10000);

	PopulateObjectInfoListFromAddresses(Conn);

	return false;
}

void GOM::GetObjectAddresses(DMA_Connection* Conn, uint32_t MaxNodes)
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
	std::println("[EFT] UpdateObjectList; {} total nodes in {}ms", NodeCount, Duration);
}

std::vector<uintptr_t> GOM::GetGameWorldAddresses()
{
	std::vector<uintptr_t> GameWorldAddresses{};

	for (auto& ObjInfo : m_ObjectInfo)
	{
		if (ObjInfo.m_ObjectName == "GameWorld") {
			GameWorldAddresses.push_back(ObjInfo.m_ObjectAddress);
		}
	}

	return GameWorldAddresses;
}

uintptr_t GOM::FindGameWorldAddressFromCache(DMA_Connection* Conn)
{
	auto GameWorldAddrs = GetGameWorldAddresses();

	auto& Proc = EFT::GetProcess();

	for (auto& GameWorldAddr : GameWorldAddrs)
	{
		auto Deref1 = Proc.ReadMem<uintptr_t>(Conn, GameWorldAddr + Offsets::CGameObject::pComponents);
		auto Deref2 = Proc.ReadMem<uintptr_t>(Conn, Deref1 + 0x18);
		auto LocalWorldAddr = Proc.ReadMem<uintptr_t>(Conn, Deref2 + Offsets::CComponent::pObjectClass);
		auto MainPlayerAddr = Proc.ReadMem<uintptr_t>(Conn, LocalWorldAddr + Offsets::CLocalGameWorld::pMainPlayer);

		if (MainPlayerAddr) {
			std::println("[EFT] LocalGameWorld found @ 0x{:X}\n", LocalWorldAddr);
			return LocalWorldAddr;
		}
	}

	throw std::runtime_error("Failed to find valid LocalGameWorld address.");
}

void GOM::DumpAllObjectsToFile(const std::string& FileName)
{
	std::ofstream OutFile(FileName, std::ios::out | std::ios::trunc);
	if (!OutFile.is_open())
	{
		std::println("[EFT] DumpAllObjectsToFile; Failed to open file: {}", FileName);
		return;
	}

	for (int i = 0; i < m_ObjectInfo.size(); i++)
	{
		auto& ObjInfo = m_ObjectInfo[i];
		OutFile << std::format("Entity #{0:d} @ {1:X} named `{2:s}`", i, ObjInfo.m_ObjectAddress, ObjInfo.m_ObjectName.c_str()) << std::endl;
	}

	OutFile.close();
}

std::vector<std::pair<uintptr_t, std::array<std::byte, 32>>> ObjectDataBuffers{};
void GOM::PopulateObjectInfoListFromAddresses(DMA_Connection* Conn)
{
	auto& Proc = EFT::GetProcess();

	ObjectDataBuffers.resize(m_ObjectAddresses.size());

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (int i = 0; i < m_ObjectAddresses.size(); i++)
	{
		auto& ObjAddr = m_ObjectAddresses[i];
		uintptr_t NameAddress = ObjAddr + Offsets::CGameObject::pName;
		VMMDLL_Scatter_PrepareEx(vmsh, NameAddress, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&ObjectDataBuffers[i].first), nullptr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (int i = 0; i < m_ObjectAddresses.size(); i++)
	{
		auto& [NameAddress, DataBuffer] = ObjectDataBuffers[i];
		VMMDLL_Scatter_PrepareEx(vmsh, NameAddress, DataBuffer.size(), reinterpret_cast<BYTE*>(DataBuffer.data()), nullptr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	m_ObjectInfo.clear();
	for (int i = 0; i < m_ObjectAddresses.size(); i++)
	{
		auto& [NameAddress, DataBuffer] = ObjectDataBuffers[i];
		std::string Name(reinterpret_cast<char*>(DataBuffer.data()), strnlen_s(reinterpret_cast<char*>(DataBuffer.data()), DataBuffer.size()));
		CObjectInfo ObjInfo{};
		ObjInfo.m_ObjectAddress = m_ObjectAddresses[i];
		ObjInfo.m_ObjectName = Name;
		m_ObjectInfo.push_back(ObjInfo);
	}
}

uintptr_t GOM::GetLatestWorldAddr(DMA_Connection* Conn)
{
	GOM::Initialize(Conn);

	uintptr_t Return{};

	try {
		Return = FindGameWorldAddressFromCache(Conn);
	}
	catch (const std::exception& e)
	{
		std::println("[EFT] GetLatestWorldAddr; Exception: {}", e.what());
	}

	return Return;
}