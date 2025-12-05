#include "pch.h"
#include "Fuser.h"
#include "Draw/Players.h"

void Fuser::Render()
{
	ImGui::SetNextWindowSize({ 1920,1080 });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 255.0f));
	ImGui::Begin("Fuser", nullptr, ImGuiWindowFlags_NoDecoration);
	auto WindowPos = ImGui::GetWindowPos();
	auto DrawList = ImGui::GetWindowDrawList();

	DrawESPPlayers::DrawAll(WindowPos, DrawList);

	ImGui::End();
	ImGui::PopStyleColor();
}