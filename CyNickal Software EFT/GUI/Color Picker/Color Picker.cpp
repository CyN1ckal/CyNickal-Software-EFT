#include "pch.h"

#include "Color Picker.h"

void ColorPicker::Render()
{
	ImGui::Begin("Color Picker");

	MyColorPicker("Enemy Color", m_EnemyColor);

	MyColorPicker("Scav Color", m_ScavColor);

	ImGui::End();
}

void ColorPicker::MyColorPicker(const char* label, ImColor& color)
{
	ImGui::SetNextItemWidth(150.0f);
	ImGui::ColorEdit4(label, (float*)&color);
}