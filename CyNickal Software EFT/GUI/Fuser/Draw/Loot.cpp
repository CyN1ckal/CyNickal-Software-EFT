#include "pch.h"
#include "GUI/Fuser/Draw/Loot.h"
#include "Game/Camera List/Camera List.h"
#include "GUI/Color Picker/Color Picker.h"
#include "Game/EFT.h"

void DrawESPLoot::DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList)
{
	if (!bMasterToggle) return;

	auto LocalPlayerPos = EFT::GetRegisteredPlayers().GetLocalPlayerPosition();

	DrawAllContainers(WindowPos, DrawList, LocalPlayerPos);
	DrawAllItems(WindowPos, DrawList, LocalPlayerPos);
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

void DrawESPLoot::DrawAllItems(const ImVec2& WindowPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos)
{
	if (!bItemToggle) return;

	auto& LootList = EFT::GetLootList();
	auto& ObservedItems = LootList.m_ObservedItems;
	std::scoped_lock lk(ObservedItems.m_Mut);
	for (auto& Loot : ObservedItems.m_Entities)
		DrawItem(Loot, DrawList, WindowPos, LocalPlayerPos);
}

void DrawESPLoot::DrawAllContainers(const ImVec2& WindowPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos)
{
	if (!bContainerToggle) return;

	auto& LootList = EFT::GetLootList();
	auto& LootableContainers = LootList.m_LootableContainers;
	std::scoped_lock lk(LootableContainers.m_Mut);
	for (auto& Container : LootableContainers.m_Entities)
		DrawContainer(Container, DrawList, WindowPos, LocalPlayerPos);
}

void DrawESPLoot::DrawItem(CObservedLootItem& Item, ImDrawList* DrawList, ImVec2 WindowPos, const Vector3& LocalPlayerPos)
{
	if (Item.IsInvalid()) return;

	if (m_MinItemPrice > 0 && Item.GetItemPrice() < m_MinItemPrice)
		return;

	Vector2 ScreenPos{};
	if (!CameraList::FPSCamera_W2S(Item.m_Position, ScreenPos))	return;

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

void DrawESPLoot::DrawContainer(CLootableContainer& Container, ImDrawList* DrawList, ImVec2 WindowPos, const Vector3& LocalPlayerPos)
{
	if (Container.IsInvalid()) return;

	Vector2 ScreenPos{};
	if (!CameraList::FPSCamera_W2S(Container.m_Position, ScreenPos))	return;

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