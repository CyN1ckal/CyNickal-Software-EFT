#include "pch.h"
#include "Radar Exfils.h"
#include "Game/Player List/Player List.h"
#include "Game/Exfil List/Exfil List.h"
#include "GUI/Radar/Radar.h"
#include "GUI/Color Picker/Color Picker.h"

void DrawRadarExfils::DrawAll(const ImVec2& WindowPos, const ImVec2& WindowSize, ImDrawList* DrawList)
{
	if (!bMasterToggle) return;

	auto LocalPlayerPos = PlayerList::GetLocalPlayerPosition();
	ImVec2 CenterScreen = { WindowPos.x + (WindowSize.x / 2.0f), WindowPos.y + (WindowSize.y / 2.0f) };

	std::scoped_lock lk(ExfilList::m_ExfilMutex);
	for (auto& Exfil : ExfilList::m_Exfils)
	{
		auto Delta3D = Exfil.m_Position - LocalPlayerPos;
		Delta3D.x *= Radar::fScale;
		Delta3D.z *= Radar::fScale;
		ImVec2 DotPosition = ImVec2(CenterScreen.x + Delta3D.z, CenterScreen.y + Delta3D.x);

		DrawList->AddCircleFilled(DotPosition, Radar::fEntityRadius, Exfil.GetRadarColor());
	}
}

void DrawRadarExfils::RenderSettings()
{
	ImGui::Checkbox("Master Exfil Toggle", &DrawRadarExfils::bMasterToggle);
}