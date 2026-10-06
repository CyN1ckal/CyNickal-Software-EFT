/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "Game/Classes/Players/CClientPlayer/CClientPlayer.h"
#include "Game/Classes/Players/CObservedPlayer/CObservedPlayer.h"

class DrawRadarPlayers
{
public:
	static void DrawAll(const ImVec2& CenterScreen, ImDrawList* DrawList);

private:
	static void Draw(const CClientPlayer& Player, const ImVec2& CenterScreen, const Vector3& LocalPos, ImDrawList* DrawList);
	static void Draw(const CObservedPlayer& Player, const ImVec2& CenterScreen, const Vector3& LocalPos, ImDrawList* DrawList);
	static void DrawViewRay(float Yaw, const ImVec2& EntityPosition, ImDrawList* DrawList, ImColor Color, float Length);
	static void DrawCharacterViewRay(const CClientPlayer& Player, const ImVec2& EntityPosition, ImDrawList* DrawList, ImColor Color, bool bIsLocalPlayer = false);
	static void DrawCharacterViewRay(const CObservedPlayer& Player, const ImVec2& EntityPosition, ImDrawList* DrawList, ImColor Color);
	static void DrawLocalPlayer(const CClientPlayer& Player, const ImVec2& CenterScreen, ImDrawList* DrawList);
};