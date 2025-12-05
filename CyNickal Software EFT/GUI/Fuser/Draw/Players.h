#pragma once
#include "Game/Classes/Players/CClientPlayer/CClientPlayer.h"
#include "Game/Classes/Players/CObservedPlayer/CObservedPlayer.h"

class DrawESPPlayers
{
public:
	static void DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList);

private:
	static void Draw(const CClientPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList);
	static void Draw(const CObservedPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList);

private: 
	static inline Vector3 m_LatestLocalPlayerPos{};
};