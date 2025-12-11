#include "pch.h"
#include "Player Table.h"
#include "Game/Player List/Player List.h"

void PlayerTable::Render()
{
	if (!bMasterToggle)
		return;

	ImGui::Begin("Player Table", &bMasterToggle);

	ImGuiTableFlags TableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_NoBordersInBody;
	if (ImGui::BeginTable("##Players", 12, TableFlags))
	{
		ImGui::TableSetupColumn("Address");
		ImGui::TableSetupColumn("Copy Addr");
		ImGui::TableSetupColumn("Position");
		ImGui::TableSetupColumn("Yaw");
		ImGui::TableSetupColumn("Side");
		ImGui::TableSetupColumn("Spawn Type");
		ImGui::TableSetupColumn("Voice");
		ImGui::TableSetupColumn("Local Player?");
		ImGui::TableSetupColumn("Tag Status");
		ImGui::TableSetupColumn("Weapon");
		ImGui::TableSetupColumn("Sanitized Weapon");
		ImGui::TableSetupColumn("Copy JOAAT");
		ImGui::TableHeadersRow();

		std::scoped_lock Lock(PlayerList::m_PlayerMutex);
		for (auto& Player : PlayerList::m_Players)
			std::visit([](auto& Player) { PlayerTable::AddRow(Player); }, Player);

		ImGui::EndTable();
	}

	ImGui::End();
}

void PlayerTable::AddRow(const CClientPlayer& Player)
{
	if (Player.IsInvalid())
		return;

	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("0x%llX", Player.m_EntityAddress);
	ImGui::TableNextColumn();
	std::string Copy = "Copy##" + std::to_string(Player.m_EntityAddress);
	if (ImGui::Button(Copy.c_str())) ImGui::SetClipboardText(std::to_string(Player.m_EntityAddress).c_str());
	ImGui::TableNextColumn();
	auto& RootPos = Player.GetBonePosition(EBoneIndex::Root);
	ImGui::Text("%.2f, %.2f, %.2f", RootPos.x, RootPos.y, RootPos.z);
	ImGui::TableNextColumn();
	ImGui::Text("%.2f", Player.m_Yaw);
	ImGui::TableNextColumn();
	ImGui::Text("%d", Player.m_Side);
	ImGui::TableNextColumn();
	ImGui::Text("%d", Player.m_SpawnType);
	ImGui::TableNextColumn();
	ImGui::Text("N/A");
	ImGui::TableNextColumn();
	ImGui::Text("%d", (Player.IsLocalPlayer()) ? 1 : 0);
	ImGui::TableNextColumn();
	ImGui::Text("N/A");
	ImGui::TableNextColumn();
	ImGui::Text((Player.m_pHands) ? Player.m_pHands->m_HeldItem.GetUnfilteredName() : "N/A");
	ImGui::TableNextColumn();
	ImGui::Text((Player.m_pHands) ? Player.m_pHands->m_HeldItem.GetSanitizedName() : "N/A");
	ImGui::TableNextColumn();
	std::string JOAAT = "JOAAT##" + std::to_string(Player.m_EntityAddress);
	if (ImGui::Button(JOAAT.c_str())) ImGui::SetClipboardText(std::format("0x{0:X}", Player.m_pHands->m_HeldItem.m_ItemHash.GetHash()).c_str());
}

void PlayerTable::AddRow(const CObservedPlayer& Player)
{
	if (Player.IsInvalid())
		return;

	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("0x%llX", Player.m_EntityAddress);
	ImGui::TableNextColumn();
	std::string Copy = "Copy##" + std::to_string(Player.m_EntityAddress);
	if (ImGui::Button(Copy.c_str())) ImGui::SetClipboardText(std::to_string(Player.m_EntityAddress).c_str());
	ImGui::TableNextColumn();
	auto& RootPos = Player.GetBonePosition(EBoneIndex::Root);
	ImGui::Text("%.2f, %.2f, %.2f", RootPos.x, RootPos.y, RootPos.z);
	ImGui::TableNextColumn();
	ImGui::Text("%.2f", Player.m_Yaw);
	ImGui::TableNextColumn();
	ImGui::Text("%d", Player.m_Side);
	ImGui::TableNextColumn();
	ImGui::Text("%d", Player.m_SpawnType);
	ImGui::TableNextColumn();
	ImGui::Text("%s", Player.m_Voice);
	ImGui::TableNextColumn();
	ImGui::Text("N/A");
	ImGui::TableNextColumn();
	ImGui::Text("%X", Player.m_TagStatus);
	ImGui::TableNextColumn();
	ImGui::Text((Player.m_pHands) ? Player.m_pHands->m_HeldItem.GetUnfilteredName() : "N/A");
	ImGui::TableNextColumn();
	ImGui::Text((Player.m_pHands) ? Player.m_pHands->m_HeldItem.GetSanitizedName() : "N/A");
	ImGui::TableNextColumn();
	std::string JOAAT = "JOAAT##" + std::to_string(Player.m_EntityAddress);
	if (ImGui::Button(JOAAT.c_str())) ImGui::SetClipboardText(std::format("0x{0:X}", Player.m_pHands->m_HeldItem.m_ItemHash.GetHash()).c_str());
}