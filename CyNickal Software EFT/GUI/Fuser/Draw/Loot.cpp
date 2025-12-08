#include "pch.h"
#include "GUI/Fuser/Draw/Loot.h"
#include "Game/Loot List/Loot List.h"
#include "Game/Camera/Camera.h"
#include "GUI/Color Picker/Color Picker.h"

void DrawESPLoot::DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList)
{
	if (!bMasterToggle) return;

	std::scoped_lock lk(LootList::m_LootMutex);

	Vector2 ScreenPos{};
	for (auto& Loot : LootList::m_LootList)
	{
		if (Loot.IsInvalid()) continue;

		if (!Camera::WorldToScreen(Loot.m_Position, ScreenPos))	continue;

		std::string Text = std::format("{0:s}", Loot.m_Name.data());

		auto TextSize = ImGui::CalcTextSize(Text.c_str());

		DrawList->AddText(
			ImVec2(WindowPos.x + ScreenPos.x - (TextSize.x / 2.0f), WindowPos.y + ScreenPos.y),
			ColorPicker::m_LootColor,
			Text.c_str()
		);
	}
}