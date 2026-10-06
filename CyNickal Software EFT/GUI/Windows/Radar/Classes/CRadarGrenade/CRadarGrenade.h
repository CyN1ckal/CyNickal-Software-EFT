/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "GUI/Windows/Radar/Classes/CRadarHoverable/CRadarHoverable.h"
#include "Game/Classes/CGrenade/CGrenade.h"

class CRadarGrenade : public CRadarHoverable {
public:
	CRadarGrenade(const CGrenade& Grenade);
	bool Draw(ImDrawList* DrawList) const;
};
