/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Fuser Exfils.h"
#include "Game/Camera List/Camera List.h"
#include "GUI/Windows/Color Picker/Color Picker.h"
#include "Game/EFT.h"
#include "GUI/MyImGui/MyImGui.h"

void FuserExfils::DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList)
{
	if (!bMasterToggle) return;

	if (EFT::pGameWorld == nullptr) return;
	if (EFT::pGameWorld->m_pExfilController == nullptr) return;

	ZoneScoped;

	auto LocalPlayerPos = EFT::GetRegisteredPlayers().GetLocalPlayerPosition();

	const auto DrawExfil = [&](const CExfilPoint& ExfilPoint) -> void {
		Vector2 ScreenPos{};

		if (!CameraList::W2S(ExfilPoint.m_Position, ScreenPos)) return;

		float Distance = LocalPlayerPos.DistanceTo(ExfilPoint.m_Position);

		CTwoToneNameplateInfo NameplateInfo{};
		NameplateInfo.Prefix = ExfilPoint.m_Name;
		NameplateInfo.Name = std::format("{:.0f}m", Distance);
		NameplateInfo.PrefixBackgroundColor = ImColor(150, 150, 150, 128);
		NameplateInfo.NameBackgroundColor = ExfilPoint.GetFuserColor();
		MyImGui::DrawTwoToneNameplate(NameplateInfo, ScreenPos);
		};

	auto& ExfilController = EFT::GetExfilController();
	ExfilController.ForEach(DrawExfil);
}



void FuserExfils::RenderSettings() {
	ImGui::Checkbox("Exfils", &bMasterToggle);
}
