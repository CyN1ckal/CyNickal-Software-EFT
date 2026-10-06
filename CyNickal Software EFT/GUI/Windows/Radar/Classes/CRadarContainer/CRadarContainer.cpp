/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CRadarContainer.h"
#include "imgui_internal.h"

CRadarContainer::CRadarContainer(const CLootableContainer& Container) : CRadarHoverable(Container.m_Position, Container.GetRadarColor()) {
	m_Name = Container.GetName();
}

bool CRadarContainer::Draw(ImDrawList* DrawList) const
{
	auto bHovered = CRadarHoverable::Draw(DrawList);

	if (bHovered) {
		ImGui::BeginTooltipEx(ImGuiTooltipFlags_OverridePrevious, 0);
		ImGui::Text("%s", m_Name.data());
		ImGui::EndTooltip();
	}

	return bHovered;
}
