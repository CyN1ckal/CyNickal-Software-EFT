/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "DMA/CDMAConnection/CDMAConnection.h"

class CKeybind
{
public:
	std::string m_Name{};
	uint32_t m_Key{ 0 };
	bool m_bTargetPC{ false };
	bool m_bRadarPC{ false };
	bool m_bWaitingForKey{ false };

public:
	void Render();
	const bool IsActive(CDMAConnection* Conn);
	const char* GetKeyName(uint32_t vkCode);
};

class Keybinds
{
public:
	static void Render();
	static void OnDMAFrame(CDMAConnection* Conn);

public:
	static inline bool bSettings{ false };
	static inline CKeybind DMARefresh = { "DMA Refresh", VK_INSERT, true, true };
	static inline CKeybind ManualRefresh = { "Manual Refresh", VK_HOME, true, true };
	static inline CKeybind Aimbot = { "Aimbot", VK_F11, true, false };
	static inline CKeybind FleaBot = { "Flea Bot", VK_PRIOR, true, true };
	static inline CKeybind OpticESP = { "Optic ESP", VK_NEXT, true, true };
	static inline CKeybind SilentAim = { "Silent Aim", VK_XBUTTON2, true, true };
};