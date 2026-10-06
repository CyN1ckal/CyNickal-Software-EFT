/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once

class Radar
{
public:
	static void Render();
	static void RenderSettings();

public:
	static inline bool bSettings{ true };
	static inline bool bMasterToggle{ true };
	static inline bool bLocalViewRay{ true };
	static inline bool bOtherPlayerViewRays{ true };
	static inline float fScale{ 3.0f };
	static inline float fLocalViewRayLength{ 100.0f };
	static inline float fOtherViewRayLength{ 33.0f };
	static inline float fEntityRadius{ 4.0f };

private: 
	static void DrawRadarMapOverlay();
};