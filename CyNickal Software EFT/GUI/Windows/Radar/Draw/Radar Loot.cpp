/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"

#include "Radar Loot.h"
#include "GUI/Windows/Color Picker/Color Picker.h"
#include "Game/EFT.h"

#include "GUI/Windows/Radar/Classes/CRadarLootItem/CRadarLootItem.h"
#include "GUI/Windows/Radar/Classes/CRadarContainer/CRadarContainer.h"
#include "GUI/Windows/Radar/Classes/CRadarCorpse/CRadarCorpse.h"

ImColor& GetRadarColorForLootItem(const CObservedLootItem& Item)
{
	if (Item.IsActiveTaskItem())
		return ColorPicker::Radar::m_QuestItems;

	return ColorPicker::Radar::m_LootColor;
}

void DrawRadarLoot::DrawAll(const ImVec2& CenterScreen, ImDrawList* DrawList)
{
	if (!bMasterToggle) return;

	ZoneScoped;

	auto LocalPlayerPos = EFT::GetRegisteredPlayers().GetLocalPlayerPosition();

	DrawAllContainers(CenterScreen, DrawList, LocalPlayerPos);
	DrawAllItems(CenterScreen, DrawList, LocalPlayerPos);
	DrawAllCorpses(CenterScreen, DrawList, LocalPlayerPos);
}

void DrawRadarLoot::RenderSettings()
{
	ImGui::Checkbox("Master Loot Toggle", &DrawRadarLoot::bMasterToggle);
	if (!DrawRadarLoot::bMasterToggle)
		return;
	ImGui::Indent();
	ImGui::Checkbox("Loot Items", &DrawRadarLoot::bLoot);
	ImGui::SetNextItemWidth(50.0f);
	ImGui::InputInt("Min Loot Price", &DrawRadarLoot::MinLootPrice, -1, 100000);
	ImGui::Checkbox("Containers", &DrawRadarLoot::bContainers);
	ImGui::Unindent();
}

void DrawRadarLoot::DrawAllCorpses(const ImVec2& CenterPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos){
	ZoneScoped;

	auto& LootList = EFT::GetLootList();

	const auto DrawCorpseItem = [&](const CCorpse& Item) -> void {
		if (Item.IsInvalid()) return;
		CRadarCorpse(Item).Draw(DrawList);
		};

	LootList.m_Corpses.ForEach_lk(DrawCorpseItem);
}

void DrawRadarLoot::DrawAllContainers(const ImVec2& CenterPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos)
{
	ZoneScoped;

	auto& LootList = EFT::GetLootList();

	const auto DrawContainerItem = [&](const CLootableContainer& Item) { CRadarContainer(Item).Draw(DrawList); };

	LootList.m_Containers.ForEach_lk(DrawContainerItem);
}

void DrawRadarLoot::DrawAllItems(const ImVec2& CenterPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos)
{
	ZoneScoped;

	auto& LootList = EFT::GetLootList();

	const auto DrawLootItem = [&](const CObservedLootItem& Item) -> void {
		if (Item.IsInvalid()) return;
		CRadarLootItem(Item).Draw(DrawList);
		};

	LootList.m_Items.ForEach_lk(DrawLootItem);
}