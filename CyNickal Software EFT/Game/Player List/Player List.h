#pragma once
#include "Game/Classes/CEFTPlayer.h"
#include "DMA/DMA.h"

class PlayerList
{
public:
	static inline std::mutex m_PlayerMutex{};
	static inline std::vector<CEFTPlayer> m_Players{};
	static inline std::vector<uintptr_t> m_PlayerAddr{};

public:
	static void QuickUpdate(DMA_Connection* Conn);
	static void FullUpdate(DMA_Connection* Conn);

	static void CompleteUpdate(DMA_Connection* Conn, uintptr_t LocalGameWorld);
	static void UpdateBaseAddresses(DMA_Connection* Conn, uintptr_t LocalGameWorld);
	static void PopulatePlayerAddresses(DMA_Connection* Conn);

private:
	static void PrintPlayers();

private:
	static inline uintptr_t m_RegisteredPlayersBaseAddress{ 0 };
	static inline uintptr_t m_PlayerDataBaseAddress{ 0 };
	static inline uint32_t m_NumPlayers{ 0 };
	static inline uint32_t m_MaxPlayers{ 0 };
};