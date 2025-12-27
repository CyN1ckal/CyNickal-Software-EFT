#include "pch.h"
#include "Fuser.h"
#include "Draw/Players.h"
#include "Draw/Loot.h"
#include "Draw/Exfils.h"
#include "GUI/Aimbot/Aimbot.h"
#include "Overlays/Ammo Count/Ammo Count.h"
#include "Game/EFT.h"
#include "Game/Camera List/Camera List.h"

void Fuser::Render()
{
	if (!bMasterToggle) return;

	if (!EFT::pGameWorld)
		return;

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(Fuser::m_ScreenSize);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 255.0f));
	ImGui::Begin("Fuser", nullptr, ImGuiWindowFlags_NoDecoration);
	auto WindowPos = ImGui::GetWindowPos();
	auto DrawList = ImGui::GetWindowDrawList();

	Aimbot::RenderFOVCircle(WindowPos, DrawList);

	if (EFT::pGameWorld->m_pLootList)
		DrawESPLoot::DrawAll(WindowPos, DrawList);

	DrawExfils::DrawAll(WindowPos, DrawList);

	if (EFT::pGameWorld->m_pRegisteredPlayers)
		DrawESPPlayers::DrawAll(WindowPos, DrawList);

	AmmoCountOverlay::Render();

	ImGui::End();
	ImGui::PopStyleColor();
}

void Fuser::RenderSettings()
{
	if (!bSettings) return;

	ImGui::Begin("Fuser Settings", &bSettings);

	ImGui::Checkbox("Optic ESP", &DrawESPPlayers::bOpticESP);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(75.0f);
	ImGui::InputScalarN("Optic Index", ImGuiDataType_U32, &CameraList::m_OpticIndex, 1);

	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.0f);
	static float fNewRadius{ 300.0f };
	if (ImGui::InputFloat("Optic Radius", &fNewRadius, 1, 5)) {
		CameraList::SetOpticRadius(fNewRadius);
	}

	ImGui::Checkbox("Player Names", &DrawESPPlayers::bNameText);
	ImGui::Checkbox("Player Skeletons", &DrawESPPlayers::bSkeleton);
	ImGui::Checkbox("Player Head Dots", &DrawESPPlayers::bHeadDot);
	DrawESPLoot::DrawSettings();

	ImGui::Checkbox("Exfils", &DrawExfils::bMasterToggle);
	ImGui::InputFloat("Screen Width", &Fuser::m_ScreenSize.x);
	ImGui::InputFloat("Screen Height", &Fuser::m_ScreenSize.y);
	ImGui::End();
}

ImVec2 Fuser::GetCenterScreen()
{
	return { m_ScreenSize.x * .5f, m_ScreenSize.y * .5f };
}