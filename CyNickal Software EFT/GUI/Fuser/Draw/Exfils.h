#pragma once

class DrawExfils
{
public:
	static void DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList);

public:
	static inline bool bMasterToggle{ true };
};