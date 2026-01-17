#include "pch.h"
#include "GUI/Radar/Radar.h"
#include "GUI/Radar/Draw/Radar Players.h"
#include "GUI/Radar/Draw/Radar Loot.h"
#include "GUI/Radar/Draw/Radar Exfils.h"
#include "Game/EFT.h"

void Radar::Render()
{
	if (!bMasterToggle) return;

	ImGui::SetNextWindowPos(ImVec2(175, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(350, 350), ImGuiCond_FirstUseEver);

	ImGui::Begin("Radar", &bMasterToggle);

	auto& io = ImGui::GetIO();
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && io.MouseWheel != 0.0f)
	{
		const float sensitivity = 0.1f;
		fScale += io.MouseWheel * sensitivity;
		fScale = std::clamp(fScale, 0.1f, 5.0f);
	}

	auto WindowPos = ImGui::GetWindowPos();
	auto WindowSize = ImGui::GetWindowSize();
	auto DrawList = ImGui::GetWindowDrawList();

	ImVec2 RectTopLeft = WindowPos;
	ImVec2 RectBottomRight = ImVec2(WindowPos.x + WindowSize.x, WindowPos.y + WindowSize.y);

	DrawList->AddRectFilled(RectTopLeft, RectBottomRight, IM_COL32(55, 55, 55, 255));

	std::scoped_lock Lock(EFT::m_GameWorldMutex);
	if (EFT::pGameWorld) {
		if (EFT::pGameWorld->m_pLootList)
			DrawRadarLoot::DrawAll(WindowPos, WindowSize, DrawList);

		if (EFT::pGameWorld->m_pExfilController)
			DrawRadarExfils::DrawAll(WindowPos, WindowSize, DrawList);

		if (EFT::pGameWorld->m_pRegisteredPlayers)
			DrawRadarPlayers::DrawAll(WindowPos, WindowSize, DrawList);
	}

	ImGui::End();
}

void Radar::RenderSettings()
{
	if (!bSettings) return;

	ImGui::Begin("Radar Settings", &bSettings);

	ImGui::Checkbox("Master Toggle", &bMasterToggle);
	if (ImGui::CollapsingHeader("General"))
	{
		ImGui::Indent();
		ImGui::SetNextItemWidth(150.0f);
		ImGui::SliderFloat("Scale", &Radar::fScale, 0.1f, 5.0f, "%.1f");
		ImGui::PushItemWidth(75.0f);
		ImGui::SliderFloat("Local View Ray Length", &Radar::fLocalViewRayLength, 10.0f, 500.0f, "%.1f");
		ImGui::SliderFloat("Other View Ray Length", &Radar::fOtherViewRayLength, 10.0f, 500.0f, "%.1f");
		ImGui::SliderFloat("Entity Radius", &Radar::fEntityRadius, 1.0f, 20.0f, "%.1f");
		ImGui::PopItemWidth();
		ImGui::Checkbox("Local Player View Ray", &Radar::bLocalViewRay);
		ImGui::Checkbox("Players View Rays", &Radar::bOtherPlayerViewRays);
		ImGui::Unindent();
	}
	DrawRadarExfils::RenderSettings();
	DrawRadarLoot::RenderSettings();

	ImGui::End();
}
