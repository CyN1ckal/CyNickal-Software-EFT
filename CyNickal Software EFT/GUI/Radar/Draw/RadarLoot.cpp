#include "pch.h"

#include "Radar Loot.h"

#include "GUI/Radar/Radar.h"
#include "Game/Loot List/Loot List.h"
#include "Game/Player List/Player List.h"
#include "GUI/Color Picker/Color Picker.h"

void DrawRadarLoot::DrawAll(const ImVec2& WindowPos, const ImVec2& WindowSize, ImDrawList* DrawList)
{
	if (!bMasterToggle) return;

	std::scoped_lock lk(LootList::m_LootMutex);

	auto LocalPlayerPos = PlayerList::GetLocalPlayerPosition();

	for (auto& Loot : LootList::m_LootList)
	{
		if (Loot.IsInvalid()) continue;

		Vector3 Delta = Loot.m_Position - LocalPlayerPos;
		Delta.x *= Radar::fScale;
		Delta.z *= Radar::fScale;
		ImVec2 DotPosition = ImVec2(WindowPos.x + (WindowSize.x / 2) + Delta.z, WindowPos.y + (WindowSize.y / 2) + Delta.x);
		DrawList->AddCircleFilled(DotPosition, 3.0f, ColorPicker::m_LootColor);
	}
}