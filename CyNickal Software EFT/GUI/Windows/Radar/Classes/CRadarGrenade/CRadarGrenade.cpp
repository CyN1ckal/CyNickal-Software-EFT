/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CRadarGrenade.h"
#include "GUI/Windows/Color Picker/Color Picker.h"
#include "imgui_internal.h"

CRadarGrenade::CRadarGrenade(const CGrenade& Grenade) : CRadarHoverable(Grenade.m_LastPosition, ColorPicker::Radar::m_GrenadeColor) {
}

bool CRadarGrenade::Draw(ImDrawList* DrawList) const
{
	auto bHovered = CRadarHoverable::Draw(DrawList);

	if (bHovered) {
		ImGui::BeginTooltipEx(ImGuiTooltipFlags_OverridePrevious, 0);
		ImGui::Text("%s", "Grenade");
		ImGui::EndTooltip();
	}

	return bHovered;
}