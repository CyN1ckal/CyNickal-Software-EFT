#include "pch.h"
#include "Players.h"
#include "Game/Player List/Player List.h"
#include "Game/Camera/Camera.h"

void DrawESPPlayers::DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList)
{
	std::scoped_lock lk(PlayerList::m_PlayerMutex);

	if (PlayerList::m_Players.empty()) return;

	auto& LocalPlayer = std::get<CClientPlayer>(PlayerList::m_Players[0]);
	if (LocalPlayer.IsInvalid()) return;

	m_LatestLocalPlayerPos = LocalPlayer.m_pSkeleton->m_BonePositions[0];

	for (int i = 1; i < PlayerList::m_Players.size(); i++)
	{
		auto& Player = PlayerList::m_Players[i];
		std::visit([WindowPos, DrawList](auto& Player) { DrawESPPlayers::Draw(Player, WindowPos, DrawList); }, Player);
	}
}

void DrawTextAtPosition(ImDrawList* DrawList, const ImVec2& Position, const ImColor& Color, const std::string& Text)
{
	auto TextSize = ImGui::CalcTextSize(Text.c_str());
	DrawList->AddText(
		ImVec2(Position.x - (TextSize.x / 2.0f), Position.y),
		Color,
		Text.c_str()
	);
}

void DrawESPPlayers::Draw(const CBaseEFTPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList)
{
	if (Player.IsInvalid())	return;

	Vector2 ScreenPos{};
	if (!Camera::WorldToScreen(Player.GetBonePosition(EBoneIndex::Root), ScreenPos)) return;

	std::string Text = std::format("{0:s} [{1:.0f}m]", Player.GetBaseName(), Player.GetBonePosition(EBoneIndex::Root).DistanceTo(m_LatestLocalPlayerPos), std::to_underlying(Player.m_SpawnType));

	DrawTextAtPosition(
		DrawList,
		ImVec2(WindowPos.x + ScreenPos.x, WindowPos.y + ScreenPos.y),
		Player.GetSideColor(),
		Text
	);

	if (!Camera::WorldToScreen(Player.GetBonePosition(EBoneIndex::Head), ScreenPos)) return;
	DrawList->AddCircle(
		ImVec2(WindowPos.x + ScreenPos.x, WindowPos.y + ScreenPos.y),
		5.0f,
		Player.GetSideColor(),
		12,
		2.0f
	);
}