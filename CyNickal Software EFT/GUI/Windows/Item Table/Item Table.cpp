/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "GUI/Windows/Item Table/Item Table.h"
#include "Game/EFT.h"

void ItemTable::Render()
{
	if (!bMasterToggle)	return;

	ZoneScoped;

	ImGui::Begin("Item Table", &bMasterToggle);

	ImGui::SetNextItemWidth(120.0f);
	ImGui::InputInt("##Price", &m_MinimumPrice, 1000, 10000);
	ImGui::SameLine();
	m_LootFilter.Draw("##ItemTableFilter", -FLT_MIN);

	ImGuiTableFlags TableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_NoBordersInBody;
	if (ImGui::BeginTable("#ItemTable", 9, TableFlags))
	{
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Distance");
		ImGui::TableSetupColumn("Price");
		ImGui::TableSetupColumn("Total Slots");
		ImGui::TableSetupColumn("Price Per Slot");
		ImGui::TableSetupColumn("Stack Count");
		ImGui::TableSetupColumn("Quest Item?");
		ImGui::TableSetupColumn("Active Task Item?");
		ImGui::TableSetupColumn("Copy Address");
		ImGui::TableHeadersRow();

		std::scoped_lock Lock(EFT::m_GameWorldMutex);
		if (EFT::pGameWorld && EFT::pGameWorld->m_pLootList && EFT::pGameWorld->m_pRegisteredPlayers) {
			auto LocalPlayerPos = EFT::GetRegisteredPlayers().GetLocalPlayerPosition();
			auto& LootList = EFT::GetLootList();
			LootList.m_Items.ForEach_lk([&](CObservedLootItem& Item) { AddRow(Item, LocalPlayerPos); });
		}

		ImGui::EndTable();
	}

	ImGui::End();
}

void ItemTable::AddRow(const CObservedLootItem& Loot, const Vector3& LocalPlayerPos)
{
	if (Loot.IsInvalid())
		return;

	auto& Name = Loot.GetName();
	auto Price = Loot.GetItemPrice();

	if (Loot.IsActiveTaskItem() == false) {
		if (Loot.GetItemPrice() < m_MinimumPrice)
			return;

		if (m_LootFilter.IsActive() && !m_LootFilter.PassFilter(Name.c_str()))
			return;
	}

	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text(Name.c_str());
	ImGui::TableNextColumn();
	ImGui::Text("%.2f m", Loot.m_Position.DistanceTo(LocalPlayerPos));
	ImGui::TableNextColumn();
	ImGui::Text("%d", Price);
	ImGui::TableNextColumn();
	ImGui::Text("%d", Loot.GetSizeInSlots());
	ImGui::TableNextColumn();
	ImGui::Text("%.2f", Loot.GetPricePerSlot());
	ImGui::TableNextColumn();
	ImGui::Text("%d", Loot.GetStackCount());
	ImGui::TableNextColumn();
	ImGui::Text(Loot.IsQuestItem() ? "True" : "False");
	ImGui::TableNextColumn();
	ImGui::Text(Loot.IsActiveTaskItem() ? "True" : "False");
	ImGui::TableNextColumn();
	if (ImGui::Button(("Copy##" + std::to_string(Loot.GetAddress())).c_str()))
		ImGui::SetClipboardText(std::format("{0:X}", Loot.GetAddress()).c_str());
}