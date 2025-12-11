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

	Vector2 ScreenPos{};
	for (auto& Loot : LootList::m_LootList)
	{
		if (Loot.IsInvalid()) continue;

		if (!Camera::WorldToScreen(Loot.m_Position, ScreenPos))	continue;

		float Distance = LocalPlayerPos.DistanceTo(Loot.m_Position);

		if (Loot.IsValuable() == false)
		{
			if (Distance > fMaxDistance)
				continue;

			if (m_LootFilter.IsActive() && !m_LootFilter.PassFilter(Loot.m_Name.data()))
				continue;
		}

		const char* ItemName = Loot.GetName();

		std::string Text = std::format("{0:s} [{1:.0f}m]", ItemName, Distance);

		auto TextSize = ImGui::CalcTextSize(Text.c_str());

		DrawList->AddText(
			ImVec2(WindowPos.x + ScreenPos.x - (TextSize.x / 2.0f), WindowPos.y + ScreenPos.y),
			Loot.GetColor(),
			Text.c_str()
		);
	}
}