/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Fuser Grenades.h"
#include "Game/Camera List/Camera List.h"
#include "Game/EFT.h"
#include "GUI/Windows/Color Picker/Color Picker.h"

void FuserGrenades::DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList)
{
	if (!bMasterToggle) return;

	if (EFT::pGameWorld == nullptr) return;
	if (EFT::pGameWorld->m_pGrenades == nullptr) return;

	ZoneScoped;

	auto LocalPlayerPos = EFT::GetRegisteredPlayers().GetLocalPlayerPosition();

	const auto DrawGrenade = [&](const CGrenade& Grenade) -> void {
		if (Grenade.IsInvalid()) return;

		Vector2 ScreenPos{};

		if (!CameraList::W2S(Grenade.m_LastPosition, ScreenPos)) return;

		float Distance = LocalPlayerPos.DistanceTo(Grenade.m_LastPosition);

		std::string Text = std::format("GRENADE [{0:.0f}m]", Distance);
		auto TextSize = ImGui::CalcTextSize(Text.c_str());
		DrawList->AddText(
			ImVec2(WindowPos.x + ScreenPos.x - (TextSize.x / 2.0f), WindowPos.y + ScreenPos.y),
			ColorPicker::Fuser::m_GrenadeColor,
			Text.c_str()
		);
		};

	EFT::pGameWorld->m_pGrenades->m_Grenades.ForEach_lk(DrawGrenade);
}

void FuserGrenades::RenderSettings() {
	ImGui::Checkbox("Grenades", &bMasterToggle);
}