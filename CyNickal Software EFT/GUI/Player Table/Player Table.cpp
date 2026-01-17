#include "pch.h"
#include "Player Table.h"
#include "Game/EFT.h"

void PlayerTable::Render()
{
	if (!bMasterToggle)
		return;

	ImGui::Begin("Player Table", &bMasterToggle);

	ImGuiTableFlags TableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_NoBordersInBody;
	if (ImGui::BeginTable("##Players", 13, TableFlags))
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
		ImGui::TableSetupColumn("Aiming");
		ImGui::TableSetupColumn("Weapon");
		ImGui::TableSetupColumn("Sanitized Weapon");
		ImGui::TableSetupColumn("Ammo");
		ImGui::TableHeadersRow();

		std::scoped_lock Lock(EFT::m_GameWorldMutex);
		if (EFT::pGameWorld && EFT::pGameWorld->m_pRegisteredPlayers) {
			auto& PlayerList = EFT::GetRegisteredPlayers();

			std::scoped_lock Lock(PlayerList.m_Mut);
			for (auto& Player : PlayerList.m_Players)
				std::visit([](auto& Player) { PlayerTable::AddRow(Player); }, Player);
		}

		ImGui::EndTable();
	}

	ImGui::End();
}

void HandRows(const CBaseEFTPlayer& Player)
{
	auto& pHands = Player.m_pHands;
	if (pHands && pHands->IsInvalid() == false && pHands->m_pHeldItem->IsInvalid() == false)
	{
		ImGui::TableNextColumn();
		ImGui::Text(pHands->m_pHeldItem->m_pItemTemplate->m_sName.c_str());

		ImGui::TableNextColumn();
		ImGui::Text(pHands->m_pHeldItem->GetItemName().c_str());

		ImGui::TableNextColumn();
		if (pHands->m_pMagazine && pHands->m_pMagazine->IsInvalid() == false)
			ImGui::Text("%d/%d, %s", pHands->m_pMagazine->m_CurrentCartridges, pHands->m_pMagazine->m_MaxCartridges, pHands->m_pMagazine->GetAmmoName().c_str());
		else
			ImGui::Text("N/A");
	}
	else
	{
		ImGui::TableNextColumn();
		ImGui::Text("N/A");
		ImGui::TableNextColumn();
		ImGui::Text("N/A");
		ImGui::TableNextColumn();
		ImGui::Text("N/A");
	}
};

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
	ImGui::Text("%s", Player.IsAiming() ? "Yes" : "No");
	HandRows(Player);
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
	ImGui::Text("N/A");
	HandRows(Player);
}