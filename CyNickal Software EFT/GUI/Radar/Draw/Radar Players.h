#pragma once
#include "Game/Classes/Players/CClientPlayer/CClientPlayer.h"
#include "Game/Classes/Players/CObservedPlayer/CObservedPlayer.h"

class DrawRadarPlayers
{
public:
	static void DrawAll(const ImVec2& WindowPos, const ImVec2& WindowSize, ImDrawList* DrawList);

private:
	static void Draw(const CClientPlayer& Player, const ImVec2& CenterScreen, const Vector3& LocalPos, ImDrawList* DrawList);
	static void Draw(const CObservedPlayer& Player, const ImVec2& CenterScreen, const Vector3& LocalPos, ImDrawList* DrawList);
	static void DrawViewRay(float Yaw, const ImVec2& EntityPosition, ImDrawList* DrawList, ImColor Color, float Length);
	static void DrawCharacterViewRay(const CClientPlayer& Player, const ImVec2& EntityPosition, ImDrawList* DrawList, ImColor Color, bool bIsLocalPlayer = false);
	static void DrawCharacterViewRay(const CObservedPlayer& Player, const ImVec2& EntityPosition, ImDrawList* DrawList, ImColor Color);
	static void DrawLocalPlayer(const CClientPlayer& Player, const ImVec2& CenterScreen, ImDrawList* DrawList);
};