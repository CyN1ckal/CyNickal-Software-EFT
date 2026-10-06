/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once

class RadarObjectives {
public:
	static void DrawAll(const ImVec2& CenterScreen, ImDrawList* DrawList);
	static void RenderSettings();

public:
	static inline bool bMasterToggle{ true };
};