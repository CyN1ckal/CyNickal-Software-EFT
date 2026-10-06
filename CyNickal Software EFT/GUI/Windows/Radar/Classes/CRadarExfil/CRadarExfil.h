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
#include "Game/Classes/CExfilPoint/CExfilPoint.h"

class CRadarExfil : public CRadarHoverable {
public:
	CRadarExfil(const CExfilPoint& ExfilPoint);
	bool Draw(ImDrawList* DrawList) const;

private:
	std::string_view m_Name{};
};