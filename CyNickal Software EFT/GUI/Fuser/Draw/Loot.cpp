#include "pch.h"
#include "GUI/Fuser/Draw/Loot.h"
#include "Game/Loot List/Loot List.h"
#include "Game/Camera/Camera.h"
#include "GUI/Color Picker/Color Picker.h"
#include "Game/Player List/Player List.h"

void DrawESPLoot::DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList)
{
	if (!bMasterToggle) return;

	std::scoped_lock lk(LootList::m_LootMutex);

	auto LocalPlayerPos = PlayerList::GetLocalPlayerPosition();

	for (auto& Loot : LootList::m_LootList)
		std::visit([&](auto& Loot) { DrawLoot(Loot, DrawList, WindowPos, LocalPlayerPos);  }, Loot);
}

void DrawESPLoot::DrawSettings()
{
	ImGui::Checkbox("Containers", &bContainerToggle);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::InputFloat("##LootContainerMaxDistance", &fMaxContainerDistance);

	ImGui::Checkbox("Items", &bItemToggle);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::InputFloat("##LootItemMaxDistance", &fMaxItemDistance);
	ImGui::InputScalar("Min Item Price", ImGuiDataType_S32, &m_MinItemPrice);
}

void DrawESPLoot::DrawLoot(CObservedLootItem& Item, ImDrawList* DrawList, ImVec2 WindowPos, Vector3 LocalPlayerPos)
{
	if (!bItemToggle) return;

	if (Item.IsInvalid()) return;

	if (m_MinItemPrice > 0 && Item.GetItemPrice() < m_MinItemPrice)
		return;

	Vector2 ScreenPos{};
	if (!Camera::WorldToScreen(Item.m_Position, ScreenPos))	return;

	auto Distance = LocalPlayerPos.DistanceTo(Item.m_Position);

	if (Distance > fMaxItemDistance)
		return;

	std::string DisplayString = std::format("{0:s} ({1:d}) [{2:.0f}m]", Item.GetName().c_str(), Item.GetItemPrice(), Distance);

	auto TextSize = ImGui::CalcTextSize(DisplayString.c_str());

	DrawList->AddText(
		ImVec2(WindowPos.x + ScreenPos.x - (TextSize.x / 2.0f), WindowPos.y + ScreenPos.y - 10.0f - TextSize.y),
		Item.GetFuserColor(),
		DisplayString.c_str()
	);
}

void DrawESPLoot::DrawLoot(CLootableContainer& Container, ImDrawList* DrawList, ImVec2 WindowPos, Vector3 LocalPlayerPos)
{
	if (!bContainerToggle) return;

	if (Container.IsInvalid()) return;

	Vector2 ScreenPos{};
	if (!Camera::WorldToScreen(Container.m_Position, ScreenPos))	return;

	auto Distance = LocalPlayerPos.DistanceTo(Container.m_Position);

	if (Distance > fMaxContainerDistance)
		return;

	std::string DisplayString = std::format("{0:s} [{1:.0f}m]", Container.GetName().c_str(), Distance);

	auto TextSize = ImGui::CalcTextSize(DisplayString.c_str());

	DrawList->AddText(
		ImVec2(WindowPos.x + ScreenPos.x - (TextSize.x / 2.0f), WindowPos.y + ScreenPos.y - 10.0f - TextSize.y),
		Container.GetFuserColor(),
		DisplayString.c_str()
	);
}