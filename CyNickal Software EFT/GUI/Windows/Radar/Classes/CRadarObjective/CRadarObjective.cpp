/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CRadarObjective.h"
#include "GUI/Windows/Color Picker/Color Picker.h"
#include "imgui_internal.h"

CRadarObjective::CRadarObjective(const CTaskZone& Zone, const CTaskObjective& Objective) : CRadarHoverable(Zone.m_Position, ColorPicker::Radar::m_Objective) {
	m_Name = Objective.m_Description;
}

bool CRadarObjective::Draw(ImDrawList* DrawList) const
{
	auto bHovered = CRadarHoverable::Draw(DrawList);

	if (bHovered) {
		ImGui::BeginTooltipEx(ImGuiTooltipFlags_OverridePrevious, 0);
		ImGui::Text("%s", "Objective");
		ImGui::Text("%s", m_Name.data());
		ImGui::EndTooltip();
	}

	return false;
}
