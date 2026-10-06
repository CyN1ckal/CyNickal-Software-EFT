/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Radar Grenades.h"
#include "Game/EFT.h"
#include "../Classes/CRadarGrenade/CRadarGrenade.h"

void DrawRadarGrenades::DrawAll(const ImVec2& CenterScreen, ImDrawList* DrawList)
{
	if (!EFT::pGameWorld || !EFT::pGameWorld->m_pGrenades) return;

	ZoneScoped;

	auto& Grenades = EFT::pGameWorld->m_pGrenades->m_Grenades;

	const auto DrawGrenadeItem = [&](const CGrenade& Item) -> void {
		if (Item.IsInvalid()) return;
		CRadarGrenade(Item).Draw(DrawList);
		};

	Grenades.ForEach_lk(DrawGrenadeItem);
}