/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include <pch.h>

enum class EMap : uint8_t
{
	FACTORY = 0,
	CUSTOMS = 1,
	WOODS = 2,
	GROUND_ZERO = 3,
	SHORELINE = 4,
	INTERCHANGE = 5,
	RESERVE = 6,
	LABS = 7,
	STREETS = 8,
	LIGHTHOUSE = 9,
	LABYRINTH = 10,
	TERMINAL = 11,

	UNKNOWN = std::numeric_limits<uint8_t>::max()
};