/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Fuser.h"
#include "Draw/Fuser Players.h"
#include "Draw/Fuser Loot.h"
#include "Draw/Fuser Exfils.h"
#include "GUI/Windows/Aimbot/Aimbot.h"
#include "Overlays/Ammo Count/Ammo Count.h"
#include "Overlays/Nearby Items/Nearby Items.h"
#include "Game/EFT.h"
#include "Game/Camera List/Camera List.h"
#include "Draw/Fuser Objectives.h"
#include "Draw/Fuser Grenades.h"
#include "Game/Exploits/Silent Aim/Silent Aim.h"

void Fuser::Render()
{
	if (!bMasterToggle) return;

	ZoneScoped;

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(Fuser::m_ScreenSize);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 255.0f));
	ImGui::Begin("Fuser", nullptr, ImGuiWindowFlags_NoDecoration);
	auto WindowPos = ImGui::GetWindowPos();
	auto DrawList = ImGui::GetWindowDrawList();

	std::scoped_lock Lock(EFT::m_GameWorldMutex);
	if (EFT::pGameWorld) {
		Aimbot::RenderFOVCircle(WindowPos, DrawList);

		SilentAim::RenderOverlay();

		DrawESPLoot::DrawAll(WindowPos, DrawList);

		FuserExfils::DrawAll(WindowPos, DrawList);

		FuserObjectives::DrawAll(WindowPos, DrawList);

		FuserGrenades::DrawAll(WindowPos, DrawList);

		DrawESPPlayers::DrawAll(WindowPos, DrawList);

		AmmoCountOverlay::Render();

		NearbyItemsOverlay::Render();
	}

	ImGui::End();
	ImGui::PopStyleColor();
}

void Fuser::RenderSettings()
{
	if (!bSettings) return;

	ImGui::Begin("Fuser Settings", &bSettings);

	if (ImGui::BeginTabBar("FuserSettingsTabs"))
	{
		if (ImGui::BeginTabItem("General"))
		{
			ImGui::Checkbox("Optic ESP", &DrawESPPlayers::bOpticESP);
			ImGui::Indent();
			ImGui::SetNextItemWidth(75.0f);
			ImGui::InputScalarN("Optic Index", ImGuiDataType_U32, &CameraList::m_OpticIndex, 1);
			ImGui::SetNextItemWidth(150.0f);
			static float fNewRadius{ 300.0f };
			if (ImGui::InputFloat("Optic Radius", &fNewRadius, 1, 5)) {
				CameraList::SetOpticRadius(fNewRadius);
			}
			ImGui::Unindent();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Players")) {
			ImGui::Checkbox("Player Names", &DrawESPPlayers::bNameText);
			ImGui::Checkbox("Player Skeletons", &DrawESPPlayers::bSkeleton);
			ImGui::Checkbox("Player Head Dots", &DrawESPPlayers::bHeadDot);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Loot & Objectives"))
		{
			DrawESPLoot::DrawSettings();
			FuserExfils::RenderSettings();
			FuserObjectives::RenderSettings();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Overlays"))
		{
			ImGui::Checkbox("Nearby Items Overlay", &NearbyItemsOverlay::bMasterToggle);
			ImGui::Indent();
			ImGui::PushItemWidth(100.0f);
			ImGui::InputFloat("Max Distance", &NearbyItemsOverlay::m_fDistance);
			ImGui::InputScalarN("Minimum Item Value", ImGuiDataType_S32, &NearbyItemsOverlay::m_MinimumItemValue, 1);
			ImGui::PopItemWidth();
			ImGui::Unindent();

			ImGui::Checkbox("Ammo Count Overlay", &AmmoCountOverlay::bMasterToggle);

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Display"))
		{
			ImGui::PushItemWidth(100.0f);
			ImGui::InputFloat("Screen Width", &Fuser::m_ScreenSize.x);
			ImGui::InputFloat("Screen Height", &Fuser::m_ScreenSize.y);
			ImGui::PopItemWidth();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

ImVec2 Fuser::GetCenterScreen()
{
	return { m_ScreenSize.x * .5f, m_ScreenSize.y * .5f };
}