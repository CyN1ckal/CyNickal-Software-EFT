#include "pch.h"
#include "Fuser.h"
#include "Draw/Players.h"
#include "Draw/Loot.h"

void Fuser::Render()
{
	ImGui::SetNextWindowSize({ 1920,1080 });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 255.0f));
	ImGui::Begin("Fuser", nullptr, ImGuiWindowFlags_NoDecoration);
	auto WindowPos = ImGui::GetWindowPos();
	auto DrawList = ImGui::GetWindowDrawList();

	DrawESPPlayers::DrawAll(WindowPos, DrawList);
	DrawESPLoot::DrawAll(WindowPos, DrawList);

	ImGui::End();
	ImGui::PopStyleColor();
}

void Fuser::RenderSettings()
{
	ImGui::Begin("Fuser Settings");
	ImGui::Checkbox("Player Names", &DrawESPPlayers::bNameText);
	ImGui::Checkbox("Player Skeletons", &DrawESPPlayers::bSkeleton);
	ImGui::Checkbox("Player Head Dots", &DrawESPPlayers::bHeadDot);
	ImGui::Checkbox("Loot", &DrawESPLoot::bMasterToggle);
	ImGui::SliderFloat("Loot Max Distance", &DrawESPLoot::fMaxDistance, 1.0f, 100.0f, "%.1f m");
	DrawESPLoot::m_LootFilter.Draw("Loot Filter");
	ImGui::End();
}
