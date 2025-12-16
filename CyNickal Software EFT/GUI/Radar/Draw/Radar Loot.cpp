#include "pch.h"

#include "Radar Loot.h"

#include "GUI/Radar/Radar.h"
#include "Game/Loot List/Loot List.h"
#include "Game/Player List/Player List.h"
#include "GUI/Color Picker/Color Picker.h"

void DrawRadarLoot::DrawAll(const ImVec2& WindowPos, const ImVec2& WindowSize, ImDrawList* DrawList)
{
	if (!bMasterToggle) return;

	auto LocalPlayerPos = PlayerList::GetLocalPlayerPosition();
	auto CenterPos = ImVec2(WindowPos.x + (WindowSize.x / 2.0f), WindowPos.y + (WindowSize.y / 2.0f));

	std::scoped_lock lk(LootList::m_LootMutex);

	for (auto& Loot : LootList::m_LootList)
		std::visit([&](auto& Loot) { Draw(Loot, CenterPos, DrawList, LocalPlayerPos); }, Loot);
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

void DrawDotAtPosition(const ImVec2& DotPos, ImDrawList* DrawList, const ImU32& Color)
{
	DrawList->AddCircleFilled(DotPos, Radar::fEntityRadius, Color);
}

void DrawRadarLoot::Draw(const CLootableContainer& Container, const ImVec2& CenterPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos)
{
	if (!bContainers) return;

	if (Container.IsInvalid()) return;

	auto Delta = Container.m_Position - LocalPlayerPos;

	Delta.x *= Radar::fScale;
	Delta.z *= Radar::fScale;

	auto DotPos = ImVec2(CenterPos.x + Delta.z, CenterPos.y + Delta.x);

	DrawDotAtPosition(DotPos, DrawList, Container.GetRadarColor());
}

void DrawRadarLoot::Draw(const CObservedLootItem& Loot, const ImVec2& CenterPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos)
{
	if (!bLoot) return;

	if (Loot.IsInvalid()) return;

	if (Loot.GetItemPrice() < MinLootPrice) return;

	auto Delta = Loot.m_Position - LocalPlayerPos;

	Delta.x *= Radar::fScale;
	Delta.z *= Radar::fScale;

	auto DotPos = ImVec2(CenterPos.x + Delta.z, CenterPos.y + Delta.x);

	DrawDotAtPosition(DotPos, DrawList, Loot.GetRadarColor());
}