/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "GUI/Windows/Radar/Classes/CRadarDrawable/CRadarDrawable.h"

class CRadarHoverable : public CRadarDrawable {
public:
	CRadarHoverable(const Vector3& WorldPos, const ImColor& PrimaryColor);
	bool Draw(ImDrawList* DrawList) const;
};