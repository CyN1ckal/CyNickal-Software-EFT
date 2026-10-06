/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Player Table.h"
#include "Game/EFT.h"

void PlayerTable::Render()
{
	if (!bMasterToggle)
		return;

	ZoneScoped;

	ImGui::Begin("Player Table", &bMasterToggle);

	ImGuiTableFlags TableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_NoBordersInBody;
	if (ImGui::BeginTable("##Players", 14, TableFlags))
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
		ImGui::TableSetupColumn("Shot ID");
		ImGui::TableHeadersRow();

		std::scoped_lock Lock(EFT::m_GameWorldMutex);
		if (EFT::pGameWorld && EFT::pGameWorld->m_pRegisteredPlayers) {
			auto& PlayerList = EFT::GetRegisteredPlayers();

			const auto DrawPlayerRow = [&](const CRegisteredPlayers::Player& Player) {
				std::visit([&](auto& Player) { PlayerTable::AddRow(Player); }, Player);
				};

			PlayerList.ForEach_lk(DrawPlayerRow);
		}

		ImGui::EndTable();
	}

	ImGui::End();
}

void HandRows(const CHandsController& pHands)
{
	if (pHands.IsInvalid() == false && pHands.m_pHeldItem->IsInvalid() == false)
	{
		ImGui::TableNextColumn();
		ImGui::Text(pHands.m_pHeldItem->m_pItemTemplate->m_sName.c_str());

		ImGui::TableNextColumn();
		ImGui::Text(pHands.m_pHeldItem->GetItemName().c_str());

		ImGui::TableNextColumn();
		if (pHands.m_pMagazine && pHands.m_pMagazine->IsInvalid() == false)
			ImGui::Text("%d/%d, %s", pHands.m_pMagazine->m_CurrentCartridges, pHands.m_pMagazine->m_MaxCartridges, pHands.m_pMagazine->GetAmmoName().c_str());
		else
			ImGui::Text("N/A");

		ImGui::TableNextColumn();
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
		ImGui::TableNextColumn();
		ImGui::Text("N/A");
	}
};

void HandRows(const CFirearmController& pHands)
{
	if (pHands.IsInvalid() == false && pHands.m_pHeldItem->IsInvalid() == false)
	{
		ImGui::TableNextColumn();
		ImGui::Text(pHands.m_pHeldItem->m_pItemTemplate->m_sName.c_str());

		ImGui::TableNextColumn();
		ImGui::Text(pHands.m_pHeldItem->GetItemName().c_str());

		ImGui::TableNextColumn();
		if (pHands.m_pMagazine && pHands.m_pMagazine->IsInvalid() == false)
			ImGui::Text("%d/%d, %s", pHands.m_pMagazine->m_CurrentCartridges, pHands.m_pMagazine->m_MaxCartridges, pHands.m_pMagazine->GetAmmoName().c_str());
		else
			ImGui::Text("N/A");

		ImGui::TableNextColumn();
		ImGui::Text("%d", pHands.m_ShotIndex);
	}
	else
	{
		ImGui::TableNextColumn();
		ImGui::Text("N/A");
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
	if (ImGui::Button(Copy.c_str())) {
		ImGui::SetClipboardText(std::format("{0:X}", Player.m_EntityAddress).c_str());
	}

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
	if (Player.m_pHands)
		HandRows(Player.m_pHands.value());
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
	if (ImGui::Button(Copy.c_str())) {
		ImGui::SetClipboardText(std::format("{0:X}", Player.m_EntityAddress).c_str());
	}
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
	if (Player.m_pHands)
		HandRows(Player.m_pHands.value());
}