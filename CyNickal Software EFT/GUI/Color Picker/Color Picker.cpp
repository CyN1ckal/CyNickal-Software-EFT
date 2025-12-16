#include "pch.h"

#include "Color Picker.h"

void ColorPicker::Render()
{
	if (!bMasterToggle)	return;

	ImGui::Begin("Color Picker", &bMasterToggle);
	Fuser::Render();
	Radar::Render();
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
		MyColorPicker("PMC Color", m_PMCColor);
		MyColorPicker("Scav Color", m_ScavColor);
		MyColorPicker("Boss Color", m_BossColor);
		MyColorPicker("Player Scav Color", m_PlayerScavColor);
		MyColorPicker("Loot Color", m_LootColor);
		MyColorPicker("Container Color", m_ContainerColor);
		MyColorPicker("Valuable Loot Color", m_ValuableLootColor);
		MyColorPicker("Exfil Color", m_ExfilColor);
		MyColorPicker("Weapon Text Color", m_WeaponTextColor);
		ImGui::Unindent();
	}
}

void ColorPicker::Radar::Render()
{
	if (ImGui::CollapsingHeader("Radar"))
	{
		ImGui::Indent();
		MyColorPicker("PMC Color", m_PMCColor);
		MyColorPicker("Scav Color", m_ScavColor);
		MyColorPicker("Boss Color", m_BossColor);
		MyColorPicker("Player Scav Color", m_PlayerScavColor);
		MyColorPicker("Local Player Color", m_LocalPlayerColor);
		MyColorPicker("Loot Color", m_LootColor);
		MyColorPicker("Container Color", m_ContainerColor);
		MyColorPicker("Valuable Loot Color", m_ValuableLootColor);
		MyColorPicker("Exfil Color", m_ExfilColor);
		ImGui::Unindent();
	}
}