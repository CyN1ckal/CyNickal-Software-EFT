/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CRadarLootItem.h"
#include "GUI/Windows/Color Picker/Color Picker.h"
#include "imgui_internal.h"

ImColor GetRadarColorForLootItem(const CObservedLootItem& Item) {
	return Item.IsActiveTaskItem() ? ColorPicker::Radar::m_QuestItems : ColorPicker::Radar::m_LootColor;
}

CRadarLootItem::CRadarLootItem(const CObservedLootItem& Item) : CRadarHoverable(Item.m_Position, GetRadarColorForLootItem(Item))
{
	m_Name = Item.GetName();
	m_Price = Item.GetItemPrice();
}

bool CRadarLootItem::Draw(ImDrawList* DrawList) const
{
	bool bHovered = CRadarHoverable::Draw(DrawList);

	if (bHovered) {
		ImGui::BeginTooltipEx(ImGuiTooltipFlags_OverridePrevious, 0);
		ImGui::Text("%s", m_Name.data());
		if (m_Price >= 0)
			ImGui::Text("Price: %d", m_Price);
		ImGui::EndTooltip();
	}

	return bHovered;
}
