#include "pch.h"
#include "Players.h"
#include "Game/Camera/Camera.h"
#include "GUI/Color Picker/Color Picker.h"

void DrawPlayers::Draw(const CBaseEFTPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList)
{
	if (Player.IsInvalid())	return;

	Vector2 ScreenPos{};
	if (!Camera::WorldToScreen(Player.m_RootPosition, ScreenPos)) return;

	static const std::string TempLabel = "Player";
	auto TextSize = ImGui::CalcTextSize(TempLabel.c_str());
	DrawList->AddText(
		ImVec2(WindowPos.x + ScreenPos.x - (TextSize.x / 2.0f), WindowPos.y + ScreenPos.y),
		ColorPicker::m_EnemyColor,
		TempLabel.c_str()
	);
}