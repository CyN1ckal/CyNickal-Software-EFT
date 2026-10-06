/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Quest List.h"
#include "Game/Quest Manager/Quest Manager.h"

const auto RenderQuestEntry = [](const CTaskEntry& Quest) -> void {
	if (Quest.m_Status != EQuestStatus::STARTED)
		return;

	if (QuestList::bHideInvalidQuests && Quest.m_Name.empty())
		return;

	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("%s", Quest.m_BSGID.c_str());
	ImGui::TableNextColumn();
	ImGui::Text("%s", Quest.m_Name.c_str());
	ImGui::TableNextColumn();
	ImGui::Text("%d", std::to_underlying(Quest.m_Status));
	ImGui::TableNextColumn();
	if (ImGui::Button(("Copy##" + Quest.m_BSGID).c_str())) {
		ImGui::SetClipboardText(Quest.m_BSGID.c_str());
	}

	ImGui::TableNextColumn();
	for (const auto& Objective : Quest.m_CompletedObjectives) {
		static const std::string CompletedLabel = "Completed:";
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), CompletedLabel.data());
		ImGui::SameLine();
		ImGui::Text("[%s] %s", Objective.m_Type.c_str(), Objective.m_Description.c_str());
		for (const auto& Zone : Objective.m_Zones) {
			ImGui::Text(" - Position: (X: %.2f, Y: %.2f, Z: %.2f) on %s", Zone.m_Position.x, Zone.m_Position.y, Zone.m_Position.z, Zone.m_MapName.c_str());
		}
		for (const auto& ItemID : Objective.m_AssociatedItems) {
			ImGui::Text(" - Associated Item: ID: %s", ItemID.c_str(), ItemID.c_str());
		}
	}
	for (const auto& Objective : Quest.m_IncompleteObjectives) {
		static const std::string IncompleteLabel = "Incomplete:";
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), IncompleteLabel.data());
		ImGui::SameLine();
		ImGui::Text("[%s] %s", Objective.m_Type.c_str(), Objective.m_Description.c_str());
		for (const auto& Position : Objective.m_Zones) {
			ImGui::Text(" - Position: (X: %.2f, Y: %.2f, Z: %.2f) on %s", Position.m_Position.x, Position.m_Position.y, Position.m_Position.z, Position.m_MapName.c_str());
		}
		for (const auto& ItemID : Objective.m_AssociatedItems) {
			ImGui::Text(" - Associated Item: %s", ItemID.c_str(), ItemID.c_str());
		}
	}

	};

void QuestList::Render() {
	if (!bQuestList) return;

	ZoneScoped;

	ImGui::Begin("Quest List");

	ImGui::Checkbox("Hide Invalid Quests", &bHideInvalidQuests);

	if (ImGui::BeginTable("##QuestListTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable)) {
		ImGui::TableSetupColumn("Quest ID", ImGuiTableColumnFlags_DefaultHide);
		ImGui::TableSetupColumn("Quest Name");
		ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_DefaultHide);
		ImGui::TableSetupColumn("Copy Quest ID", ImGuiTableColumnFlags_DefaultHide);
		ImGui::TableSetupColumn("Objectives");
		ImGui::TableHeadersRow();

		QuestManager::ForEachQuest(RenderQuestEntry);

		ImGui::EndTable();
	}

	ImGui::End();
}