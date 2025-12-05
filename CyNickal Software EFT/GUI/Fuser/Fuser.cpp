#include "pch.h"
#include "Fuser.h"
#include "Game/Player List/Player List.h"
#include "Draw/Players.h"

void Fuser::Render()
{
	ImGui::SetNextWindowSize({ 1920,1080 });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 255.0f));
	ImGui::Begin("Fuser", nullptr, ImGuiWindowFlags_NoDecoration);
	auto WindowPos = ImGui::GetWindowPos();
	auto DrawList = ImGui::GetWindowDrawList();

	std::scoped_lock lk(PlayerList::m_PlayerMutex);
	for (auto& Player : PlayerList::m_Players)
		std::visit([WindowPos, DrawList](auto Player) {DrawPlayers::Draw(Player, WindowPos, DrawList); }, Player);

	ImGui::End();
	ImGui::PopStyleColor();
}