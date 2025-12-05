#include "pch.h"
#include "GUI/Radar/Radar.h"
#include "GUI/Radar/Draw/Player.h"

void Radar::Render()
{
	ImGui::Begin("Radar");

	auto WindowPos = ImGui::GetWindowPos();
	auto WindowSize = ImGui::GetWindowSize();
	auto DrawList = ImGui::GetWindowDrawList();

	ImVec2 RectTopLeft = WindowPos;
	ImVec2 RectBottomRight = ImVec2(WindowPos.x + WindowSize.x, WindowPos.y + WindowSize.y);

	DrawList->AddRectFilled(RectTopLeft, RectBottomRight, IM_COL32(55, 55, 55, 255));

	DrawRadarPlayers::DrawAll();

	ImGui::End();
}

void Radar::RenderSettings()
{
	ImGui::Begin("Radar Settings");

	ImGui::SliderFloat("Scale", &Radar::fScale, 0.1f, 5.0f, "%.1f");
	ImGui::SliderFloat("Local View Ray Length", &Radar::fLocalViewRayLength, 10.0f, 500.0f, "%.1f");
	ImGui::SliderFloat("Other View Ray Length", &Radar::fOtherViewRayLength, 10.0f, 500.0f, "%.1f");
	ImGui::Checkbox("Local Player View Ray", &Radar::bLocalViewRay);
	ImGui::Checkbox("Players View Rays", &Radar::bOtherPlayerViewRays);

	ImGui::End();
}
