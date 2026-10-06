/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once

class Fuser
{
public:
	static void Render();
	static void RenderSettings();
	static ImVec2 GetCenterScreen();


public:
	static inline bool bSettings{ true };
	static inline bool bMasterToggle{ true };
	static inline ImVec2 m_ScreenSize{ 1920.0f,1080.0f };

};