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
		MyColorPicker("PMC Color##Fuser", m_PMCColor);
		MyColorPicker("Scav Color##Fuser", m_ScavColor);
		MyColorPicker("Boss Color##Fuser", m_BossColor);
		MyColorPicker("Player Scav Color##Fuser", m_PlayerScavColor);
		MyColorPicker("Loot Color##Fuser", m_LootColor);
		MyColorPicker("Container Color##Fuser", m_ContainerColor);
		MyColorPicker("Exfil Color##Fuser", m_ExfilColor);
		MyColorPicker("Weapon Text Color##Fuser", m_WeaponTextColor);
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
		ImGui::Unindent();
	}
}