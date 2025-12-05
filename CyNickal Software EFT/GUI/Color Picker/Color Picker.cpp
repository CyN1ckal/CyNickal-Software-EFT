#include "pch.h"

#include "Color Picker.h"

void ColorPicker::Render()
{
	ImGui::Begin("Color Picker");

	MyColorPicker("PMC Color", m_PMCColor);
	MyColorPicker("Scav Color", m_ScavColor);
	MyColorPicker("Player Scav Color", m_PlayerScavColor);
	MyColorPicker("Local Player Color", m_LocalPlayerColor);

	ImGui::End();
}

void ColorPicker::MyColorPicker(const char* label, ImColor& color)
{
	ImGui::SetNextItemWidth(150.0f);
	ImGui::ColorEdit4(label, (float*)&color);
}