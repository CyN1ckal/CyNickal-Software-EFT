/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Nearby Items.h"
#include "Game/EFT.h"

void NearbyItemsOverlay::Render() {
	if (!bMasterToggle)
		return;

	if (!EFT::pGameWorld->m_pRegisteredPlayers || !EFT::pGameWorld->m_pLootList)
		return;

	auto LocalPlayerPos = EFT::GetRegisteredPlayers().GetLocalPlayerPosition();
	auto& LootList = EFT::GetLootList();

	struct NearbyItem {
		std::string Name;
		int32_t Price;
		float Distance;
		std::string DisplayString;
	};

	std::vector<NearbyItem> NearbyItems;
	LootList.m_Items.ForEach_lk([&](CObservedLootItem& Item) {
		if (Item.IsInvalid()) return;

		if (Item.GetItemPrice() < m_MinimumItemValue) return;

		auto Distance = LocalPlayerPos.DistanceTo(Item.m_Position);
		if (Distance > m_fDistance) return;

		auto DisplayString = std::format("{0:s} ({1:d}) [{2:.0f}m]", Item.GetName().c_str(), Item.GetItemPrice(), Distance);
		NearbyItems.push_back({ Item.GetName(), Item.GetItemPrice(), Distance, std::move(DisplayString) });
		});

	if (NearbyItems.empty()) return;

	std::ranges::sort(NearbyItems, std::greater{}, &NearbyItem::Price);

	const auto& Style = ImGui::GetStyle();
	float LineHeight = ImGui::GetTextLineHeightWithSpacing();
	float HeaderHeight = LineHeight + Style.ItemSpacing.y + 2.0f; // title + separator
	float ContentHeight = HeaderHeight + (LineHeight * NearbyItems.size()) + (Style.WindowPadding.y * 2.0f);

	float MaxTextWidth = ImGui::CalcTextSize("Nearby Items (25m)").x;
	for (const auto& Item : NearbyItems) {
		float TextWidth = ImGui::CalcTextSize(Item.DisplayString.c_str()).x;
		if (TextWidth > MaxTextWidth)
			MaxTextWidth = TextWidth;
	}
	float WidgetWidth = MaxTextWidth + (Style.WindowPadding.x * 2.0f);

	ImVec2 ParentSize = ImGui::GetWindowSize();
	float PosX = 10.0f;
	float PosY = (ParentSize.y - ContentHeight) * 0.5f;

	ImGui::SetCursorPos(ImVec2(PosX, PosY));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 0.85f));

	if (ImGui::BeginChild("NearbyItemsChild", ImVec2(WidgetWidth, ContentHeight), true)) {
		ImGui::Text("Nearby Items (25m)");
		ImGui::Separator();

		for (const auto& Item : NearbyItems) {
			ImGui::TextUnformatted(Item.DisplayString.c_str());
		}
	}
	ImGui::EndChild();

	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
}