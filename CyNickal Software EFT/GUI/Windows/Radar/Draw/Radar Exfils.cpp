/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Radar Exfils.h"
#include "Game/EFT.h"
#include "GUI/Windows/Radar/Classes/CRadarExfil/CRadarExfil.h"

void DrawRadarExfils::DrawAll(const ImVec2& CenterScreen, ImDrawList* DrawList)
{
	if (!bMasterToggle) return;

	ZoneScoped;

	auto& ExfilController = EFT::GetExfilController();

	const auto DrawExfil = [&](const CExfilPoint& ExfilPoint) {	CRadarExfil(ExfilPoint).Draw(DrawList);	};
	ExfilController.ForEach(DrawExfil);
}

void DrawRadarExfils::RenderSettings()
{
	ImGui::Checkbox("Master Exfil Toggle", &DrawRadarExfils::bMasterToggle);
}