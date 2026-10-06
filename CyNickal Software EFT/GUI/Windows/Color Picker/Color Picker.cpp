/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"

#include "Color Picker.h"

void ColorPicker::Render()
{
	if (!bMasterToggle)	return;

	ImGui::Begin("Color Picker", &bMasterToggle);
	ColorPicker::Fuser::Render();
	ColorPicker::Radar::Render();
	ImGui::End();
}

void ColorPicker::MyColorPicker(const char* label, ImColor& color)
{
	ImGui::SetNextItemWidth(150.0f);
	ImGui::ColorEdit4(label, &color.Value.x);
}

void ColorPicker::Fuser::Render()
{
	if (ImGui::CollapsingHeader("Fuser"))
	{
		ImGui::Indent();
		MyColorPicker("PMC Color##Fuser", m_PMCColor);
		MyColorPicker("Scav Color##Fuser", m_ScavColor);
		MyColorPicker("Boss Color##Fuser", m_BossColor);
		MyColorPicker("Player Scav Color##Fuser", m_PlayerScavColor);
		MyColorPicker("Loot Color##Fuser", m_LootColor);
		MyColorPicker("Container Color##Fuser", m_ContainerColor);
		MyColorPicker("Exfil Color##Fuser", m_ExfilColor);
		MyColorPicker("Weapon Text Color##Fuser", m_WeaponTextColor);
		MyColorPicker("Objective Color##Fuser", m_Objective);
		MyColorPicker("Quest Items Color##Fuser", m_QuestItems);
		MyColorPicker("Corpse Color##Fuser", m_CorpseColor);
		MyColorPicker("Grenade Color##Fuser", m_GrenadeColor);
		ImGui::Unindent();
	}
}

void ColorPicker::Radar::Render()
{
	if (ImGui::CollapsingHeader("Radar"))
	{
		ImGui::Indent();
		MyColorPicker("PMC Color##Radar", m_PMCColor);
		MyColorPicker("Scav Color##Radar", m_ScavColor);
		MyColorPicker("Boss Color##Radar", m_BossColor);
		MyColorPicker("Player Scav Color##Radar", m_PlayerScavColor);
		MyColorPicker("Local Player Color##Radar", m_LocalPlayerColor);
		MyColorPicker("Loot Color##Radar", m_LootColor);
		MyColorPicker("Container Color##Radar", m_ContainerColor);
		MyColorPicker("Exfil Color##Radar", m_ExfilColor);
		MyColorPicker("Objective Color##Radar", m_Objective);
		MyColorPicker("Quest Items Color##Radar", m_QuestItems);
		MyColorPicker("Corpse Color##Radar", m_CorpseColor);
		MyColorPicker("Grenade Color##Radar", m_GrenadeColor);
		ImGui::Unindent();
	}
}