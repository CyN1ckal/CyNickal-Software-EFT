/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CRadarExfil.h"
#include "GUI/Windows/Color Picker/Color Picker.h"
#include "imgui_internal.h"

CRadarExfil::CRadarExfil(const CExfilPoint& ExfilPoint) : CRadarHoverable(ExfilPoint.m_Position, ColorPicker::Radar::m_ExfilColor) {
	m_Name = ExfilPoint.m_Name;
}

bool CRadarExfil::Draw(ImDrawList* DrawList) const
{
	auto bHovered = CRadarHoverable::Draw(DrawList);

	if (bHovered) {
		ImGui::BeginTooltipEx(ImGuiTooltipFlags_OverridePrevious, 0);
		ImGui::Text("%s", "Exfil Point");
		ImGui::Text("%s", m_Name.data());
		ImGui::EndTooltip();
	}

	return bHovered;
}