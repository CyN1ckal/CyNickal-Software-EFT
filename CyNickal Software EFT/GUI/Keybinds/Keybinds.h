#pragma once
#include "DMA/DMA.h"

class CKeybind
{
public:
	std::string m_Name{};
	uint32_t m_Key{ 0 };
	bool m_bTargetPC{ false };
	bool m_bRadarPC{ false };

public:
	void Render();
	const bool IsActive(DMA_Connection* Conn);
};

class Keybinds
{
public:
	static void Render();
	static void OnDMAFrame(DMA_Connection* Conn);

public:
	static inline bool bSettings{ true };

private:
	static inline CKeybind DMARefresh = { "DMA Refresh", VK_INSERT, true, true };
	static inline CKeybind PlayerRefresh = { "Player Refresh", VK_HOME, true, true };
	static inline CKeybind Aimbot = { "Aimbot", VK_XBUTTON2, true, false };
};