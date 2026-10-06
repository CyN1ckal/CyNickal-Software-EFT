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

class Aimbot
{
public:
	static void RenderSettings();
	static void RenderFOVCircle(const ImVec2& WindowPos, ImDrawList* DrawList);
	static void OnDMAFrame(CDMAConnection* Conn);

public:
	static inline bool bSettings{ false };
	static inline bool bMasterToggle{ false };
	static inline bool bDrawFOV{ true };
	static inline float fSmoothX{ 10.0f };
	static inline float fSmoothY{ 10.0f };
	static inline float fPixelFOV{ 75.0f };
	static inline float fDeadzoneFov{ 2.0f };

private:
	static ImVec2 GetAimDeltaToTarget(uintptr_t TargetAddress);
	static uintptr_t FindBestTarget();
};