#include "pch.h"
#include "Player.h"
#include "GUI/Radar/Radar.h"
#include "GUI/Color Picker/Color Picker.h"
#include "Game/Player List/Player List.h"

void DrawRadarPlayers::DrawAll()
{
	std::scoped_lock lk(PlayerList::m_PlayerMutex);

	if (PlayerList::m_Players.empty())
		return;

	auto WindowPos = ImGui::GetWindowPos();
	auto WindowSize = ImGui::GetWindowSize();
	auto DrawList = ImGui::GetWindowDrawList();
	auto CenterScreen = ImVec2(WindowPos.x + (WindowSize.x / 2), WindowPos.y + (WindowSize.y / 2));
	auto& LocalPlayer = std::get<CClientPlayer>(PlayerList::m_Players[0]);

	if (LocalPlayer.IsInvalid())
		return;

	auto LocalPos = LocalPlayer.GetBonePosition(EBoneIndex::Root);

	DrawLocalPlayer(LocalPlayer, CenterScreen, DrawList);

	for (int i = 1; i < PlayerList::m_Players.size(); i++)
	{
		auto& Player = PlayerList::m_Players[i];
		std::visit([CenterScreen, DrawList, LocalPos](auto& Player) { Draw(Player, CenterScreen, LocalPos, DrawList);  }, Player);
	}
}

void DrawRadarPlayers::Draw(const CClientPlayer& Player, const ImVec2& CenterScreen, const Vector3& LocalPos, ImDrawList* DrawList)
{
	if (Player.IsInvalid())
		return;

	auto& PlayerPos = Player.GetBonePosition(EBoneIndex::Root);
	auto Delta3D = PlayerPos - LocalPos;

	Delta3D.x *= Radar::fScale;
	Delta3D.z *= Radar::fScale;

	auto Color = Player.GetSideColor();

	ImVec2 DotPosition = ImVec2(CenterScreen.x + Delta3D.z, CenterScreen.y + Delta3D.x);
	DrawList->AddCircleFilled(DotPosition, 5, Color);

	if (Radar::bOtherPlayerViewRays)
		DrawCharacterViewRay(Player, DotPosition, DrawList, Color);
}

void DrawRadarPlayers::Draw(const CObservedPlayer& Player, const ImVec2& CenterScreen, const Vector3& LocalPos, ImDrawList* DrawList)
{
	if (Player.IsInvalid())
		return;

	auto& PlayerPos = Player.GetBonePosition(EBoneIndex::Root);
	auto Delta3D = PlayerPos - LocalPos;

	Delta3D.x *= Radar::fScale;
	Delta3D.z *= Radar::fScale;

	auto Color = Player.GetSideColor();

	ImVec2 DotPosition = ImVec2(CenterScreen.x + Delta3D.z, CenterScreen.y + Delta3D.x);
	DrawList->AddCircleFilled(DotPosition, 5, Color);

	if (Radar::bOtherPlayerViewRays)
		DrawCharacterViewRay(Player, DotPosition, DrawList, Color);
}

void DrawRadarPlayers::DrawViewRay(float Yaw, const ImVec2& EntityPosition, ImDrawList* DrawList, ImColor Color, float Length)
{
	constexpr float AnglesToRadians = 0.01745329f;
	float LocalYawInRadians = Yaw * AnglesToRadians;
	auto ViewRayEndPos = ImVec2(
		EntityPosition.x + (std::cos(LocalYawInRadians) * Length),
		EntityPosition.y + (std::sin(LocalYawInRadians) * Length)
	);
	DrawList->AddLine(EntityPosition, ViewRayEndPos, Color, 2.0f);
}

void DrawRadarPlayers::DrawCharacterViewRay(const CObservedPlayer& Player, const ImVec2& EntityPosition, ImDrawList* DrawList, ImColor Color)
{
	DrawViewRay(Player.m_Yaw, EntityPosition, DrawList, Color, Radar::fOtherViewRayLength);
}

void DrawRadarPlayers::DrawCharacterViewRay(const CClientPlayer& Player, const ImVec2& EntityPosition, ImDrawList* DrawList, ImColor Color, bool bIsLocalPlayer)
{
	DrawViewRay(Player.m_Yaw, EntityPosition, DrawList, Color, (bIsLocalPlayer) ? Radar::fLocalViewRayLength : Radar::fOtherViewRayLength);
}

void DrawRadarPlayers::DrawLocalPlayer(const CClientPlayer& Player, const ImVec2& CenterScreen, ImDrawList* DrawList)
{
	DrawList->AddCircleFilled(CenterScreen, 5, ColorPicker::m_LocalPlayerColor);

	DrawCharacterViewRay(Player, CenterScreen, DrawList, ColorPicker::m_LocalPlayerColor, true);
}