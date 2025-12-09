#include "pch.h"
#include "GUI/Loot Table/Loot Table.h"
#include "Game/Loot List/Loot List.h"
#include "Game/Player List/Player List.h"

void LootTable::Render()
{
	if (!bMasterToggle)	return;

	ImGui::Begin("Loot Table", &bMasterToggle);

	m_LootFilter.Draw("##LootTableFilter", -FLT_MIN);

	if (ImGui::BeginTable("#LootTable", 3))
	{
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Position");
		ImGui::TableSetupColumn("Distance");
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
		}

		ImGui::EndTable();
	}

	ImGui::End();
}