#include "pch.h"
#include "Exfils.h"
#include "Game/Camera List/Camera List.h"
#include "GUI/Color Picker/Color Picker.h"
#include "Game/EFT.h"

void DrawExfils::DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList)
{
	if (!bMasterToggle) return;

	if (EFT::pGameWorld == nullptr) return;
	if (EFT::pGameWorld->m_pExfilController == nullptr) return;

	auto& ExfilController = EFT::GetExfilController();
	std::scoped_lock lk(ExfilController.m_ExfilMutex);

	Vector2 ScreenPos{};

	auto LocalPlayerPos = EFT::GetRegisteredPlayers().GetLocalPlayerPosition();

	for (auto& Exfil : ExfilController.m_Exfils)
	{
		if (!CameraList::W2S(Exfil.m_Position, ScreenPos)) continue;

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