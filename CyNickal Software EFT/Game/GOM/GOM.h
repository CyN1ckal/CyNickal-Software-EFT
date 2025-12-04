#pragma once
#include "DMA/DMA.h"
#include "Game/Classes/CObjectInfo.h"

class GOM
{
public:
	static bool Initialize(DMA_Connection* Conn);
	static uintptr_t GetLocalGameWorldAddr(DMA_Connection* Conn);

private:
	static inline uintptr_t GameObjectManagerAddress{ 0 };
	static inline uintptr_t LastActiveNode{ 0 };
	static inline uintptr_t ActiveNodes{ 0 };
	static inline std::vector<uintptr_t> m_ObjectAddresses{};
	static inline std::vector<CObjectInfo> m_ObjectInfo{};

	static void GetObjectAddresses(DMA_Connection* Conn, uint32_t MaxNodes = std::numeric_limits<uint32_t>::max());
	static std::vector<uintptr_t> GetGameWorldAddresses(DMA_Connection* Conn);
	static void DumpAllObjectsToFile(const std::string& FileName);
	static void PopulateObjectInfoListFromAddresses(DMA_Connection* Conn);
};