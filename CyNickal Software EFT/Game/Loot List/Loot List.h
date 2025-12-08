#pragma once
#include "Game/Classes/CLoot/CLoot.h"
#include "DMA/DMA.h"

class LootList
{
public:
	static inline std::mutex m_LootMutex{};
	static inline std::vector<CLoot> m_LootList{};

public:
	static void CompleteUpdate(DMA_Connection* Conn, uintptr_t LocalGameWorld);
	static void FullUpdate(DMA_Connection* Conn);
	static void QuickUpdate(DMA_Connection* Conn);
	
private:
	static inline uintptr_t m_pLootListAddress{ 0 };
	static inline uintptr_t m_BaseLootListAddress{ 0 };
	static inline uint32_t m_LootNum{ 0 };
	static inline uint32_t m_LootMax{ 0 };
};