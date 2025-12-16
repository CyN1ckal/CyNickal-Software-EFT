#include "pch.h"
#include "Exfils.h"
#include "Game/Exfil List/Exfil List.h"
#include "Game/Player List/Player List.h"
#include "Game/Camera/Camera.h"
#include "GUI/Color Picker/Color Picker.h"

void DrawExfils::DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList)
{
	if (!bMasterToggle) return;

	std::scoped_lock lk(ExfilList::m_ExfilMutex);

	Vector2 ScreenPos{};

	auto LocalPlayerPos = PlayerList::GetLocalPlayerPosition();

	for (auto& Exfil : ExfilList::m_Exfils)
	{
		if (!Camera::WorldToScreen(Exfil.m_Position, ScreenPos)) continue;

		float Distance = LocalPlayerPos.DistanceTo(Exfil.m_Position);

		std::string Text = std::format("{0:s} [{1:.0f}m]", Exfil.m_Name, Distance);
		auto TextSize = ImGui::CalcTextSize(Text.c_str());
		DrawList->AddText(
			ImVec2(WindowPos.x + ScreenPos.x - (TextSize.x / 2.0f), WindowPos.y + ScreenPos.y),
			Exfil.GetFuserColor(),
			Text.c_str()
		);
	}
}