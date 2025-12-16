#include "pch.h"
#include "Radar Players.h"
#include "GUI/Radar/Radar.h"
#include "GUI/Color Picker/Color Picker.h"
#include "Game/Player List/Player List.h"

void DrawRadarPlayers::DrawAll(const ImVec2& WindowPos, const ImVec2& WindowSize, ImDrawList* DrawList)
{
	auto LocalPos = PlayerList::GetLocalPlayerPosition();

	std::scoped_lock lk(PlayerList::m_PlayerMutex);

	if (PlayerList::m_Players.empty())
		return;

	auto CenterScreen = ImVec2(WindowPos.x + (WindowSize.x / 2), WindowPos.y + (WindowSize.y / 2));

	for (auto& Player : PlayerList::m_Players)
	{
		std::visit([CenterScreen, DrawList, LocalPos](auto& Player) {
			Draw(Player, CenterScreen, LocalPos, DrawList);
			}, Player);
	}
}

void DrawRadarPlayers::Draw(const CClientPlayer& Player, const ImVec2& CenterScreen, const Vector3& LocalPos, ImDrawList* DrawList)
{
	if (Player.IsInvalid())
		return;

	if (Player.IsLocalPlayer())
	{
		DrawLocalPlayer(Player, CenterScreen, DrawList);
		return;
	}

	auto& PlayerPos = Player.GetBonePosition(EBoneIndex::Root);
	auto Delta3D = PlayerPos - LocalPos;

	Delta3D.x *= Radar::fScale;
	Delta3D.z *= Radar::fScale;

	auto Color = Player.GetRadarColor();

	ImVec2 DotPosition = ImVec2(CenterScreen.x + Delta3D.z, CenterScreen.y + Delta3D.x);
	DrawList->AddCircleFilled(DotPosition, Radar::fEntityRadius, Color);

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

	auto Color = Player.GetRadarColor();

	ImVec2 DotPosition = ImVec2(CenterScreen.x + Delta3D.z, CenterScreen.y + Delta3D.x);
	DrawList->AddCircleFilled(DotPosition, Radar::fEntityRadius, Color);

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
	DrawList->AddCircleFilled(CenterScreen, 5, ColorPicker::Radar::m_LocalPlayerColor);

	DrawCharacterViewRay(Player, CenterScreen, DrawList, ColorPicker::Radar::m_LocalPlayerColor, true);
}