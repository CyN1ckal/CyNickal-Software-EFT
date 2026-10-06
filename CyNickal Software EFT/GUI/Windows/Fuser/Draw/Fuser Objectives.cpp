/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Fuser Objectives.h"
#include "Game/Quest Manager/Quest Manager.h"
#include "Game/Camera List/Camera List.h"
#include "Game/EFT.h"
#include "GUI/Windows/Color Picker/Color Picker.h"

void FuserObjectives::DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList)
{
	if (!bMasterToggle)
		return;

	ZoneScoped;

	auto CurrentMap = EFT::pGameWorld->m_CurrentMap;

	auto LocalPlayerPos = EFT::GetRegisteredPlayers().GetLocalPlayerPosition();

	const auto DrawObjective = [&](const CTaskEntry& Task) -> void {

		if (Task.m_Status != EQuestStatus::STARTED)
			return;

		for (auto& Objective : Task.m_IncompleteObjectives) {

			for (auto& Zone : Objective.m_Zones) {

				if (Zone.m_Map != CurrentMap)
					continue;

				Vector2 ScreenPos{};

				if (!CameraList::W2S(Zone.m_Position, ScreenPos)) {
					continue;
				}

				auto Distance = LocalPlayerPos.DistanceTo(Zone.m_Position);

				auto TextSize = ImGui::CalcTextSize(Objective.m_Description.c_str());

				auto TextPos = ImVec2(WindowPos.x + ScreenPos.x - (TextSize.x / 2.0f), WindowPos.y + ScreenPos.y);

				DrawList->AddText(TextPos, ColorPicker::Fuser::m_Objective, std::format("{0:s} [{1:.0f}m]", Objective.m_Description.c_str(), Distance).c_str());
			}
		}

		};

	QuestManager::ForEachQuest(DrawObjective);
}

void FuserObjectives::RenderSettings()
{
	ImGui::Checkbox("Objectives", &bMasterToggle);
}
