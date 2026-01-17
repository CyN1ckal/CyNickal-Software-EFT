#include "pch.h"
#include "GUI/Item Table/Item Table.h"
#include "Game/EFT.h"

void ItemTable::Render()
{
	if (!bMasterToggle)	return;


	ImGui::Begin("Item Table", &bMasterToggle);

	ImGui::SetNextItemWidth(120.0f);
	ImGui::InputInt("##Price", &m_MinimumPrice, 1000, 10000);
	ImGui::SameLine();
	m_LootFilter.Draw("##ItemTableFilter", -FLT_MIN);

	ImGuiTableFlags TableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_NoBordersInBody;
	if (ImGui::BeginTable("#ItemTable", 6, TableFlags))
	{
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Distance");
		ImGui::TableSetupColumn("Price");
		ImGui::TableSetupColumn("Total Slots");
		ImGui::TableSetupColumn("Price Per Slot");
		ImGui::TableSetupColumn("Stack Count");
		ImGui::TableHeadersRow();

		std::scoped_lock Lock(EFT::m_GameWorldMutex);
		if (EFT::pGameWorld && EFT::pGameWorld->m_pLootList && EFT::pGameWorld->m_pRegisteredPlayers) {
			auto LocalPlayerPos = EFT::GetRegisteredPlayers().GetLocalPlayerPosition();
			auto& LootList = EFT::GetLootList();
			std::scoped_lock lk(LootList.m_ObservedItems.m_Mut);
			for (auto& Item : LootList.m_ObservedItems.m_Entities)
				AddRow(Item, LocalPlayerPos);
		}

		ImGui::EndTable();
	}

	ImGui::End();
}

void ItemTable::AddRow(const CObservedLootItem& Loot, const Vector3& LocalPlayerPos)
{
	if (Loot.IsInvalid())
		return;

	auto Price = Loot.GetItemPrice();
	if (Loot.GetItemPrice() < m_MinimumPrice)
		return;

	auto Name = Loot.GetName();
	if (m_LootFilter.IsActive() && !m_LootFilter.PassFilter(Name.c_str()))
		return;

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
}