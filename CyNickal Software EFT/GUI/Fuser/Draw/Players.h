#pragma once
#include "Game/Classes/Players/CBaseEFTPlayer/CBaseEFTPlayer.h"

class DrawPlayers
{
public:
	static void Draw(const CBaseEFTPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList);
};