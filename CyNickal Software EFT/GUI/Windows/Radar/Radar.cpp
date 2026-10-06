/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "GUI/Windows/Radar/Radar.h"
#include "GUI/Windows/Radar/Draw/Radar Players.h"
#include "GUI/Windows/Radar/Draw/Radar Loot.h"
#include "GUI/Windows/Radar/Draw/Radar Exfils.h"
#include "GUI/Windows/Radar/Draw/Radar Objectives.h"
#include "GUI/Windows/Radar/Draw/Radar Grenades.h"
#include "Game/EFT.h"
#include "GUI/Windows/Main Window/Main Window.h"
#include "GUI/Windows/Radar/Classes/CRadarDrawable/CRadarDrawable.h"

void Radar::Render()
{
	if (!bMasterToggle) return;

	ZoneScoped;

	ImGui::SetNextWindowPos(ImVec2(175, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(350, 350), ImGuiCond_FirstUseEver);

	ImGui::Begin("Radar", &bMasterToggle, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

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
	auto CenterScreen = ImVec2(WindowPos.x + (WindowSize.x * 0.5f), WindowPos.y + (WindowSize.y * 0.5f));

	ImVec2 RectTopLeft = WindowPos;
	ImVec2 RectBottomRight = ImVec2(WindowPos.x + WindowSize.x, WindowPos.y + WindowSize.y);

	DrawList->AddRectFilled(RectTopLeft, RectBottomRight, IM_COL32(25, 25, 25, 255));

	std::scoped_lock Lock(EFT::m_GameWorldMutex);
	if (EFT::pGameWorld && EFT::pGameWorld->m_pRegisteredPlayers) {

		CRadarDrawable::SetLocalPlayerPos(EFT::GetRegisteredPlayers().GetLocalPlayerPosition());
		CRadarDrawable::SetCenterRadar(CenterScreen);

		DrawRadarMapOverlay();

		if (EFT::pGameWorld->m_pLootList) {
			DrawRadarLoot::DrawAll(CenterScreen, DrawList);
		}

		if (EFT::pGameWorld->m_pExfilController) {
			DrawRadarExfils::DrawAll(CenterScreen, DrawList);
		}

		DrawRadarPlayers::DrawAll(CenterScreen, DrawList);
		RadarObjectives::DrawAll(CenterScreen, DrawList);
		DrawRadarGrenades::DrawAll(CenterScreen, DrawList);
	}

	ImGui::End();
}

ImVec2 GetUVFromWorldCoords(const Vector3& WorldCoords, const Vector3& MapTopLeftCoords, const Vector3& MapBottomRightCoords) {
	float MapWidth = MapBottomRightCoords.x - MapTopLeftCoords.x;
	float MapHeight = MapTopLeftCoords.z - MapBottomRightCoords.z;
	float U = (WorldCoords.x - MapTopLeftCoords.x) / MapWidth;
	float V = (MapTopLeftCoords.z - WorldCoords.z) / MapHeight;
	return ImVec2(U, V);
}

/*
	All coordinates are stored in game world space.
	The UVTopLeft and UVBottomRight are the world coordinates that correspond to the top-left and bottom-right of the map texture.
*/
struct CMapCoordinates {
	Vector3 UVTopLeft{};
	Vector3 UVBottomRight{};
};

CMapCoordinates& GetMapCoords(EMap Map);

void Radar::DrawRadarMapOverlay() {
	if (!EFT::pGameWorld->m_MapTexture.pTexture) return;

	auto WindowSize = ImGui::GetWindowSize();

	auto LocalPlayerPos = EFT::GetRegisteredPlayers().GetLocalPlayerPosition();

	const float RadarWidthGameUnits = WindowSize.x / fScale;
	const float RadarHeightGameUnits = WindowSize.y / fScale;

	auto MapCoords = GetMapCoords(EFT::pGameWorld->m_CurrentMap);

	ImVec2 TopLeftUV = GetUVFromWorldCoords({ LocalPlayerPos.x - (RadarWidthGameUnits * 0.5f), 0, LocalPlayerPos.z + (RadarHeightGameUnits * 0.5f) }, MapCoords.UVTopLeft, MapCoords.UVBottomRight);
	ImVec2 BottomRightUV = GetUVFromWorldCoords({ LocalPlayerPos.x + (RadarWidthGameUnits * 0.5f), 0, LocalPlayerPos.z - (RadarHeightGameUnits * 0.5f) }, MapCoords.UVTopLeft, MapCoords.UVBottomRight);
	
	ImGui::SetCursorPos({ 0.0f,0.0f });
	ImGui::Image(EFT::pGameWorld->m_MapTexture.pTexture, WindowSize, TopLeftUV, BottomRightUV);
}

void MapCoordEditor();
void Radar::RenderSettings()
{
	if (!bSettings) return;

	ImGui::Begin("Radar Settings", &bSettings);

	ImGui::Checkbox("Master Toggle", &bMasterToggle);
	if (ImGui::CollapsingHeader("General"))
	{
		ImGui::Indent();
		ImGui::SetNextItemWidth(150.0f);
		ImGui::SliderFloat("Scale", &Radar::fScale, 0.25f, 20.0f, "%.1f");
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
	RadarObjectives::RenderSettings();

	if (ImGui::CollapsingHeader("Map Coordinates Editor")) {
		MapCoordEditor();
	}

	ImGui::End();
}

CMapCoordinates Factory{
{ -64.79f, 0.f, 67.25f },
{ 74.88f, 0.f, -64.41f }
};

CMapCoordinates Customs{
{ -376.578, 0.f, 266.773f },
{ 703.473, 0.f, -304.574 }
};

CMapCoordinates Interchange{
{ -564.5f, 0.f, 443.52f },
{ 576.87f, 0.f, -506.96f }
};

CMapCoordinates Woods{
{ -774.36f, 0.f, 446.34f },
{ 645.61f, 0.f, -916.23f }
};

CMapCoordinates Reserve{
{ -311.48f, 0.f, 246.74f },
{ 292.16f, 0.f, -280.44f }
};

CMapCoordinates Shoreline{
{ -1102.05f, 0.f, 652.54f },
{ 522.19f, 0.f, -470.62f }
};

CMapCoordinates GroundZero{
{ -101.88f, 0.f, 366.95f },
{ 251.17, 0.f, -126.4f }
};

CMapCoordinates Streets{
{ -280.53f, 0.f, 533.18f },
{ 325.11f, 0.f, -298.83f }
};

CMapCoordinates Default{
{ 0.f, 0.f, 0.f },
{ 0.f, 0.f, 0.f }
};

CMapCoordinates& GetMapCoords(EMap Map) {
	switch (Map) {
	case EMap::CUSTOMS:
		return Customs;
	case EMap::FACTORY:
		return Factory;
	case EMap::INTERCHANGE:
		return Interchange;
	case EMap::WOODS:
		return Woods;
	case EMap::RESERVE:
		return Reserve;
	case EMap::SHORELINE:
		return Shoreline;
	case EMap::GROUND_ZERO:
		return GroundZero;
	case EMap::STREETS:
		return Streets;
	default:
		return Default;
	}
}

void MapCoordEditor() {
	auto& Coords = GetMapCoords(EFT::GetCurrentMap());
	ImGui::InputFloat3("Top Left", &Coords.UVTopLeft.x);
	ImGui::InputFloat3("Bottom Right", &Coords.UVBottomRight.x);
}