/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CRadarDrawable.h"
#include "GUI/Windows/Radar/Radar.h"

CRadarDrawable::CRadarDrawable(const Vector3& WorldPos, const ImColor& PrimaryColor) : m_PrimaryColor(PrimaryColor) {
	auto WorldDelta = WorldPos - m_LocalPlayerPos;
	WorldDelta.x *= Radar::fScale;
	WorldDelta.z *= Radar::fScale;

	m_ScreenPos = ImVec2(m_CenterRadar.x + WorldDelta.x, m_CenterRadar.y - WorldDelta.z);
}

void CRadarDrawable::Draw(ImDrawList* DrawList) const {
	DrawList->AddCircleFilled(m_ScreenPos, m_Radius, m_PrimaryColor);
}