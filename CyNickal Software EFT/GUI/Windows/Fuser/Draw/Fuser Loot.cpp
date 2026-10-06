/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "GUI/Windows/Fuser/Draw/Fuser Loot.h"
#include "Game/Camera List/Camera List.h"
#include "GUI/Windows/Color Picker/Color Picker.h"
#include "Game/EFT.h"
#include "GUI/MyImGui/MyImGui.h"

ImColor& GetColorForLoot(const CObservedLootItem& Item)
{
	if (Item.IsActiveTaskItem())
		return ColorPicker::Fuser::m_QuestItems;

	return ColorPicker::Fuser::m_LootColor;
}

void DrawESPLoot::DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList)
{
	if (!bMasterToggle) return;

	if (!EFT::pGameWorld->m_pLootList) return;

	ZoneScoped;

	auto LocalPlayerPos = EFT::GetRegisteredPlayers().GetLocalPlayerPosition();

	DrawAllContainers(WindowPos, DrawList, LocalPlayerPos);
	DrawAllItems(WindowPos, DrawList, LocalPlayerPos);
	DrawAllCorpses(WindowPos, DrawList, LocalPlayerPos);
}

void DrawESPLoot::DrawSettings()
{
	ImGui::Checkbox("Containers", &bContainerToggle);
	ImGui::Indent();
	ImGui::SetNextItemWidth(100.0f);
	ImGui::InputFloat("Max Distance (m)##Container", &fMaxContainerDistance);
	ImGui::Unindent();

	ImGui::Checkbox("Items", &bItemToggle);
	ImGui::Indent();
	ImGui::SetNextItemWidth(100.0f);
	ImGui::InputFloat("Max Distance (m)##Item", &fMaxItemDistance);
	ImGui::SetNextItemWidth(100.0f);
	ImGui::InputScalar("Min Item Price", ImGuiDataType_S32, &m_MinItemPrice);
	ImGui::Unindent();
}

void DrawESPLoot::DrawAllItems(const ImVec2& WindowPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos)
{
	if (!bItemToggle) return;

	ZoneScoped;

	auto& LootList = EFT::GetLootList();
	auto& ObservedItems = LootList.m_Items;

	const auto Drawer = [&](CObservedLootItem& Item) -> void {
		if (Item.IsInvalid()) return;
		DrawItem(Item, DrawList, WindowPos, LocalPlayerPos);
		};

	ObservedItems.ForEach_lk(Drawer);
}

void DrawESPLoot::DrawAllContainers(const ImVec2& WindowPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos)
{
	if (!bContainerToggle) return;

	ZoneScoped;

	auto& LootList = EFT::GetLootList();
	auto& LootableContainers = LootList.m_Containers;

	LootableContainers.ForEach_lk([&](CLootableContainer& Container) { DrawContainer(Container, DrawList, WindowPos, LocalPlayerPos); });
}

void DrawESPLoot::DrawAllCorpses(const ImVec2& WindowPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos)
{
	ZoneScoped;

	auto& LootList = EFT::GetLootList();
	auto& Corpses = LootList.m_Corpses;

	Corpses.ForEach_lk([&](CCorpse& Corpse) { DrawCorpse(Corpse, DrawList, WindowPos, LocalPlayerPos); });
}

void DrawESPLoot::DrawCorpse(CCorpse& Corpse, ImDrawList* DrawList, ImVec2 WindowPos, const Vector3& LocalPlayerPos) {
	if (Corpse.IsInvalid()) return;

	Vector2 ScreenPos{};
	if (!CameraList::W2S(Corpse.m_Position, ScreenPos)) return;

	auto Distance = LocalPlayerPos.DistanceTo(Corpse.m_Position);

	CTwoToneNameplateInfo NameplateInfo{};
	NameplateInfo.Prefix = "Corpse";
	NameplateInfo.Name = std::format("{:.0f}m", Distance);
	NameplateInfo.PrefixBackgroundColor = ImColor(150, 150, 150, 128);
	NameplateInfo.NameBackgroundColor = ColorPicker::Fuser::m_CorpseColor;
	MyImGui::DrawTwoToneNameplate(NameplateInfo, ScreenPos);
}

void DrawESPLoot::DrawItem(CObservedLootItem& Item, ImDrawList* DrawList, ImVec2 WindowPos, const Vector3& LocalPlayerPos)
{
	if (Item.IsInvalid()) return;

	auto Distance = LocalPlayerPos.DistanceTo(Item.m_Position);

	if (Item.IsActiveTaskItem() == false) {
		if (Item.GetItemPrice() < m_MinItemPrice)
			return;

		if (Distance > fMaxItemDistance)
			return;
	}

	Vector2 ScreenPos{};
	if (!CameraList::W2S(Item.m_Position, ScreenPos)) return;

	std::string DisplayString = std::format("{0:s} ({1:d}) [{2:.0f}m]", Item.GetName().c_str(), Item.GetItemPrice(), Distance);

	auto TextSize = ImGui::CalcTextSize(DisplayString.c_str());

	DrawList->AddText(
		ImVec2(WindowPos.x + ScreenPos.x - (TextSize.x / 2.0f), WindowPos.y + ScreenPos.y),
		GetColorForLoot(Item),
		DisplayString.c_str()
	);
}

void DrawESPLoot::DrawContainer(CLootableContainer& Container, ImDrawList* DrawList, ImVec2 WindowPos, const Vector3& LocalPlayerPos)
{
	if (Container.IsInvalid()) return;

	auto Distance = LocalPlayerPos.DistanceTo(Container.m_Position);

	if (Distance > fMaxContainerDistance)
		return;

	Vector2 ScreenPos{};
	if (!CameraList::W2S(Container.m_Position, ScreenPos))	return;

	std::string DisplayString = std::format("{0:s} [{1:.0f}m]", Container.GetName().c_str(), Distance);

	auto TextSize = ImGui::CalcTextSize(DisplayString.c_str());

	DrawList->AddText(
		ImVec2(WindowPos.x + ScreenPos.x - (TextSize.x / 2.0f), WindowPos.y + ScreenPos.y),
		Container.GetFuserColor(),
		DisplayString.c_str()
	);
}