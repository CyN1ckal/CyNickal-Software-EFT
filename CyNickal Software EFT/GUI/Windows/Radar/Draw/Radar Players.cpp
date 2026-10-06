/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Radar Players.h"
#include "GUI/Windows/Radar/Radar.h"
#include "GUI/Windows/Color Picker/Color Picker.h"
#include "Game/EFT.h"

void DrawRadarPlayers::DrawAll(const ImVec2& CenterScreen, ImDrawList* DrawList)
{
	ZoneScoped;

	auto& PlayerList = EFT::GetRegisteredPlayers();

	auto LocalPos = PlayerList.GetLocalPlayerPosition();

	const auto DrawRadarPlayer = [&](const CRegisteredPlayers::Player& Player) {
		std::visit([CenterScreen, DrawList, LocalPos](auto& Player) { Draw(Player, CenterScreen, LocalPos, DrawList);	}, Player);
		};

	PlayerList.ForEach_lk(DrawRadarPlayer);
}

void DrawRadarPlayers::Draw(const CClientPlayer& Player, const ImVec2& CenterScreen, const Vector3& LocalPos, ImDrawList* DrawList)
{
	if (Player.IsInvalid())
		return;

	if (Player.IsDead())
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

	ImVec2 DotPosition = ImVec2(CenterScreen.x + Delta3D.x, CenterScreen.y - Delta3D.z);
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

	ImVec2 DotPosition = ImVec2(CenterScreen.x + Delta3D.x, CenterScreen.y - Delta3D.z);
	DrawList->AddCircleFilled(DotPosition, Radar::fEntityRadius, Color);

	if (Radar::bOtherPlayerViewRays)
		DrawCharacterViewRay(Player, DotPosition, DrawList, Color);
}

void DrawRadarPlayers::DrawViewRay(float Yaw, const ImVec2& EntityPosition, ImDrawList* DrawList, ImColor Color, float Length)
{
	constexpr float AnglesToRadians = 0.01745329f;
	float LocalYawInRadians = Yaw * AnglesToRadians;
	auto ViewRayEndPos = ImVec2(
		EntityPosition.x + (std::sin(LocalYawInRadians) * Length),
		EntityPosition.y - (std::cos(LocalYawInRadians) * Length)
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