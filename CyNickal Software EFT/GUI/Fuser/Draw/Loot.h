#pragma once

class DrawESPLoot
{
public:
	static void DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList);

public:
	static inline bool bMasterToggle{ true };
	static inline float fMaxDistance{ 25.0f };
	static inline ImGuiTextFilter m_LootFilter{};
};