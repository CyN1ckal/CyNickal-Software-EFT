/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "Game/Classes/CTaskZone/CTaskZone.h"

class CTaskObjective {
public:
	std::vector<CTaskZone> m_Zones{};
	std::vector<std::string> m_AssociatedItems{};
	std::string m_BSGID{};
	std::string m_Description{};
	std::string m_Type{};

public:
	CTaskObjective(const std::string& BSGID);
};