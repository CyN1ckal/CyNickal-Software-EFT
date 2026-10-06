/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "pch.h"

inline float ImDistance(const ImVec2& p1, const ImVec2& p2) {
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	return std::sqrt(dx * dx + dy * dy);
}