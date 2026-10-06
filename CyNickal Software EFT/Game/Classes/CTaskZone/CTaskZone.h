/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "Game/Classes/Vector.h"
#include "Game/Enums/EMap.h"
#include "json.hpp"

class CTaskZone {
public:
	Vector3 m_Position{};
	std::string m_MapName{};
	EMap m_Map{ EMap::UNKNOWN };

public:
	CTaskZone(const nlohmann::json& ZoneJson);
	static std::vector<CTaskZone> CreateZoneVector(const std::string& ObjectiveID);
};