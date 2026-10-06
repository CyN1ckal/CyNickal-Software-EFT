/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CRadarHoverable.h"
#include "GUI/Utils/ImDistance.hpp"

CRadarHoverable::CRadarHoverable(const Vector3& WorldPos, const ImColor& PrimaryColor) : CRadarDrawable(WorldPos, PrimaryColor) {

}

bool CRadarHoverable::Draw(ImDrawList* DrawList) const {

	CRadarDrawable::Draw(DrawList);

	auto& MousePos = ImGui::GetIO().MousePos;
	float Distance = ImDistance(m_ScreenPos, MousePos);
	bool bHovered = Distance <= m_Radius;

	return bHovered;
}