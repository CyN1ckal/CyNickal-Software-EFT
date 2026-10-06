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

enum class EExfilStatus : uint32_t
{
	NotPresent = 1,
	UncompleteRequirements = 2,
	Countdown = 3,
	Regular = 4,
	Pending = 5,
	AwaitingActivation = 6,
	Hidden = 7,
	UNKNOWN = std::numeric_limits<uint32_t>::max()
};