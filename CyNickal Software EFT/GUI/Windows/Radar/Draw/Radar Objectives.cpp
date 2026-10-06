/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Radar Objectives.h"
#include "Game/Quest Manager/Quest Manager.h"
#include "Game/EFT.h"
#include "GUI/Windows/Radar/Classes/CRadarObjective/CRadarObjective.h"

void RadarObjectives::DrawAll(const ImVec2& CenterScreen, ImDrawList* DrawList)
{
	if (!bMasterToggle)
		return;

	ZoneScoped;

	auto LocalPos = EFT::GetRegisteredPlayers().GetLocalPlayerPosition();
	auto& CurrentMap = EFT::pGameWorld->m_CurrentMap;

	const auto DrawRadarObjective = [&](const CTaskEntry& Task) -> void {
		if (Task.m_Status != EQuestStatus::STARTED)
			return;

		if (Task.m_IncompleteObjectives.empty())
			return;

		for (const auto& Objective : Task.m_IncompleteObjectives) {
			for (const auto& Zone : Objective.m_Zones) {
				if (Zone.m_Map != CurrentMap)
					continue;

				CRadarObjective(Zone, Objective).Draw(DrawList);
			}
		};

		};

	QuestManager::ForEachQuest(DrawRadarObjective);
}

void RadarObjectives::RenderSettings() {
	ImGui::Checkbox("Objectives", &bMasterToggle);
}