#pragma once

class DrawRadarExfils
{
public:
	static inline bool bMasterToggle{ true };

public:
	static void DrawAll(const ImVec2& WindowPos, const ImVec2& WindowSize, ImDrawList* DrawList);
	static void RenderSettings();
};