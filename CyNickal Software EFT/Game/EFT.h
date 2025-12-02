#pragma once

#include "DMA/DMA.h"
#include "DMA/Process.h"

#include "Classes/CObjectInfo.h"
class EFT
{
public:
	static bool Initialize(DMA_Connection* Conn);
	static const Process& GetProcess();

private:
	static uintptr_t GetLocalGameWorldAddr(DMA_Connection* Conn);
	static uintptr_t GetGameWorldAddr(DMA_Connection* Conn);
	static inline Process Proc{};

private:
	static inline uintptr_t GameObjectManagerAddress{ 0 };
	static inline uintptr_t LastActiveNode{ 0 };
	static inline uintptr_t ActiveNodes{ 0 };

private:
	static void GetObjectAddresses(DMA_Connection* Conn, uint32_t MaxNodes = std::numeric_limits<uint32_t>::max());
	static inline std::vector<uintptr_t> m_ObjectAddresses{};

public:
	static void DumpAllObjectsToFile(const std::string& FileName);
	static void PopulateObjectInfoListFromAddresses(DMA_Connection* Conn);
	static inline std::vector<CObjectInfo> m_ObjectInfo{};
};