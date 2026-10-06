/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include <cstdint>
#include <limits>

enum class EPlayerSide : uint32_t
{
	USEC = 0,
	BEAR = 2,
	SCAV = 4,
	UNKNOWN = std::numeric_limits<uint32_t>::max()
};