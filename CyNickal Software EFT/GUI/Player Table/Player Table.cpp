#include "pch.h"
#include "Player Table.h"
#include "Game/Player List/Player List.h"

void PlayerTable::Render()
{
	ImGui::Begin("Player Table");

	if (ImGui::BeginTable("##Players", 4))
	{
		ImGui::TableSetupColumn("Address");
		ImGui::TableSetupColumn("Position");
		ImGui::TableSetupColumn("Yaw");
		ImGui::TableSetupColumn("Side");
		ImGui::TableHeadersRow();

		std::scoped_lock Lock(PlayerList::m_PlayerMutex);
		for (auto& Player : PlayerList::m_Players)
		{
			std::visit([](auto& Player) {
				if (Player.IsInvalid())
					return;

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("0x%llX", Player.m_EntityAddress);
				ImGui::TableNextColumn();
				ImGui::Text("%.2f, %.2f, %.2f", Player.m_RootPosition.x, Player.m_RootPosition.y, Player.m_RootPosition.z);
				ImGui::TableNextColumn();
				ImGui::Text("%.2f", Player.m_Yaw);
				ImGui::TableNextColumn();
				ImGui::Text("%d", Player.m_Side);
				}
			, Player);
		}
	}
	ImGui::EndTable();

	ImGui::End();
}