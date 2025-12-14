#include "pch.h"
#include "Fuser.h"
#include "Draw/Players.h"
#include "Draw/Loot.h"
#include "Draw/Exfils.h"
#include "GUI/Aimbot/Aimbot.h"
#include "Overlays/Ammo Count/Ammo Count.h"

void Fuser::Render()
{
	if (!bMasterToggle) return;

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(Fuser::m_ScreenSize);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 255.0f));
	ImGui::Begin("Fuser", nullptr, ImGuiWindowFlags_NoDecoration);
	auto WindowPos = ImGui::GetWindowPos();
	auto DrawList = ImGui::GetWindowDrawList();

	Aimbot::RenderFOVCircle(WindowPos, DrawList);
	DrawESPLoot::DrawAll(WindowPos, DrawList);
	DrawExfils::DrawAll(WindowPos, DrawList);
	DrawESPPlayers::DrawAll(WindowPos, DrawList);
	AmmoCountOverlay::Render();

	ImGui::End();
	ImGui::PopStyleColor();
}

void Fuser::RenderSettings()
{
	if (!bSettings) return;

	ImGui::Begin("Fuser Settings", &bSettings);
	ImGui::Checkbox("Player Names", &DrawESPPlayers::bNameText);
	ImGui::Checkbox("Player Skeletons", &DrawESPPlayers::bSkeleton);
	ImGui::Checkbox("Player Head Dots", &DrawESPPlayers::bHeadDot);
	ImGui::Checkbox("Loot", &DrawESPLoot::bMasterToggle);
	ImGui::Checkbox("Exfils", &DrawExfils::bMasterToggle);
	ImGui::SliderFloat("Loot Max Distance", &DrawESPLoot::fMaxDistance, 1.0f, 100.0f, "%.1f m");
	DrawESPLoot::m_LootFilter.Draw("Loot Filter");
	ImGui::InputFloat("Screen Width", &Fuser::m_ScreenSize.x);
	ImGui::InputFloat("Screen Height", &Fuser::m_ScreenSize.y);
	ImGui::End();
}

ImVec2 Fuser::GetCenterScreen()
{
	return { m_ScreenSize.x * .5f, m_ScreenSize.y * .5f };
}