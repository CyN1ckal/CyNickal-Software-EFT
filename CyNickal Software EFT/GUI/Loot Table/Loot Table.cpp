#include "pch.h"
#include "GUI/Loot Table/Loot Table.h"
#include "Game/Loot List/Loot List.h"
#include "Game/Player List/Player List.h"

void LootTable::Render()
{
	if (!bMasterToggle)	return;

	ImGui::Begin("Loot Table", &bMasterToggle);

	m_LootFilter.Draw("##LootTableFilter", -FLT_MIN);
	ImGui::Checkbox("Items", &bItems); ImGui::SameLine();
	ImGui::Checkbox("Containers", &bContainers);

	ImGuiTableFlags TableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_NoBordersInBody;
	if (ImGui::BeginTable("#LootTable", 6, TableFlags))
	{
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Distance");
		ImGui::TableSetupColumn("Price");
		ImGui::TableSetupColumn("Total Slots");
		ImGui::TableSetupColumn("Price Per Slot");
		ImGui::TableSetupColumn("Stack Count");
		ImGui::TableHeadersRow();

		auto LocalPlayerPos = PlayerList::GetLocalPlayerPosition();

		std::scoped_lock lk(LootList::m_LootMutex);
		for (auto& Loot : LootList::m_LootList)
			std::visit([&](auto& item) { AddRow(item, LocalPlayerPos); }, Loot);

		ImGui::EndTable();
	}

	ImGui::End();
}

void LootTable::AddRow(const CLootableContainer& Container, const Vector3& LocalPlayerPos)
{
	if (!bContainers)
		return;

	if (Container.IsInvalid())
		return;

	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text(Container.GetName().c_str());
	ImGui::TableNextColumn();
	ImGui::Text("%.2f m", Container.m_Position.DistanceTo(LocalPlayerPos));
	ImGui::TableNextColumn();
	ImGui::Text("N/A");
	ImGui::TableNextColumn();
	ImGui::Text("N/A");
	ImGui::TableNextColumn();
	ImGui::Text("N/A");
	ImGui::TableNextColumn();
	ImGui::Text("N/A");
}

void LootTable::AddRow(const CObservedLootItem& Loot, const Vector3& LocalPlayerPos)
{
	if (!bItems)
		return;

	if (Loot.IsInvalid())
		return;

	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text(Loot.GetName().c_str());
	ImGui::TableNextColumn();
	ImGui::Text("%.2f m", Loot.m_Position.DistanceTo(LocalPlayerPos));
	ImGui::TableNextColumn();
	ImGui::Text("%d", Loot.GetItemPrice());
	ImGui::TableNextColumn();
	ImGui::Text("%d", Loot.GetSizeInSlots());
	ImGui::TableNextColumn();
	ImGui::Text("%.2f", Loot.GetPricePerSlot());
	ImGui::TableNextColumn();
	ImGui::Text("%d", Loot.GetStackCount());
}