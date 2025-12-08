#pragma once

class DrawRadarLoot
{
public:
	static void DrawAll(const ImVec2& WindowPos, const ImVec2& WindowSize, ImDrawList* DrawList);

public:
	static inline bool bMasterToggle{ true };
};