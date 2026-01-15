#pragma once
#include "DMA/DMA.h"
#include "Game/Classes/CObjectInfo.h"

class GOM
{
public:
	static bool Initialize(DMA_Connection* Conn);
	static uintptr_t FindGameWorldAddressFromCache(DMA_Connection* Conn);
	static const uintptr_t& GetMainPlayerAddress() { return m_MainPlayerAddress; }
public:
	static inline uintptr_t GameObjectManagerAddress{ 0 };
	static inline uintptr_t LastActiveNode{ 0 };
	static inline uintptr_t ActiveNodes{ 0 };
private:
	static inline uintptr_t m_MainPlayerAddress{ 0 };
	static inline std::vector<uintptr_t> m_ObjectAddresses{};
	static inline std::vector<CObjectInfo> m_ObjectInfo{};

public:
	static void GetObjectAddresses(DMA_Connection* Conn, uint32_t MaxNodes = std::numeric_limits<uint32_t>::max());
private:
	static std::vector<uintptr_t> GetGameWorldAddresses(DMA_Connection* Conn);
	static void DumpAllObjectsToFile(const std::string& FileName);
public:
	static void PopulateObjectInfoListFromAddresses(DMA_Connection* Conn);
};