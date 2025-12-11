#include "pch.h"
#include "GUI/Loot Table/Loot Table.h"
#include "Game/Loot List/Loot List.h"
#include "Game/Player List/Player List.h"

void LootTable::Render()
{
	if (!bMasterToggle)	return;

	ImGui::Begin("Loot Table", &bMasterToggle);

	m_LootFilter.Draw("##LootTableFilter", -FLT_MIN);

	ImGuiTableFlags TableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_NoBordersInBody;
	if (ImGui::BeginTable("#LootTable", 5, TableFlags))
	{
		ImGui::TableSetupColumn("Raw Name");
		ImGui::TableSetupColumn("Position");
		ImGui::TableSetupColumn("Distance");
		ImGui::TableSetupColumn("Sanitized Name");
		ImGui::TableSetupColumn("JOAAT");
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableHeadersRow();

		auto LocalPlayerPos = PlayerList::GetLocalPlayerPosition();

		std::scoped_lock lk(LootList::m_LootMutex);
		for (auto& Loot : LootList::m_LootList)
		{
			if (Loot.IsInvalid()) continue;

			if (m_LootFilter.IsActive() && !m_LootFilter.PassFilter(Loot.m_Name.data()))
				continue;

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text(Loot.m_Name.data());
			ImGui::TableNextColumn();
			ImGui::Text("%.0f %.0f %.0f", Loot.m_Position.x, Loot.m_Position.y, Loot.m_Position.z);
			ImGui::TableNextColumn();
			ImGui::Text("%.0fm", LocalPlayerPos.DistanceTo(Loot.m_Position));
			ImGui::TableNextColumn();
			auto ItemName = Loot.GetName();
			if (ItemName)
				ImGui::Text(Loot.GetName());
			ImGui::TableNextColumn();
			std::string Joaat = "Copy##" + std::to_string(Loot.m_EntityAddress);
			if (ImGui::Button(Joaat.c_str()))
				ImGui::SetClipboardText(std::format("0x{0:X}", Loot.m_ItemHash.GetHash()).c_str());
		}

		ImGui::EndTable();
	}

	ImGui::End();
}