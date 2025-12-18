#include "pch.h"
#include "Ammo Count.h"
#include "Game/Player List/Player List.h"

struct MagazineInfo
{
	uint32_t CurrentCartridges{ 0 };
	uint32_t MaxCartridges{ 0 };
	std::string AmmoTypeName{};
};

MagazineInfo GetLocalPlayerMagazine()
{
	MagazineInfo info{};

	std::scoped_lock Lock(PlayerList::m_PlayerMutex);
	auto LocalPlayer = PlayerList::GetLocalPlayer();

	if (!LocalPlayer || LocalPlayer->IsInvalid() || !LocalPlayer->m_pHands)
		return info;

	auto& Magazine = LocalPlayer->m_pHands->m_pMagazine;

	if (!Magazine)
		return info;

	info.CurrentCartridges = Magazine->m_CurrentCartridges;
	info.MaxCartridges = Magazine->m_MaxCartridges;
	info.AmmoTypeName = Magazine->GetAmmoName();

	return info;
}

void AmmoCountOverlay::Render()
{
	auto WindowSize = ImGui::GetWindowSize();
	auto WindowPos = ImGui::GetWindowPos();

	auto Magazine = GetLocalPlayerMagazine();
	
	if (Magazine.MaxCartridges == 0) return;

	std::string AmmoString = std::format("{}/{}", Magazine.CurrentCartridges, Magazine.MaxCartridges);

	ImGui::PushFont(nullptr, 32.0f);
	auto TextSize = (AmmoString.size() > Magazine.AmmoTypeName.size()) ? ImGui::CalcTextSize(AmmoString.c_str()) : ImGui::CalcTextSize(Magazine.AmmoTypeName.c_str());

	ImGui::SetNextWindowPos({ WindowPos.x + (WindowSize.x * 0.5f) - (TextSize.x * 0.5f), WindowPos.y + WindowSize.y - 80.0f});
	ImGui::SetNextWindowSize({ TextSize.x + 8.0f, TextSize.y * 2.0f + 8.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 4.0f,4.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2());
	ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2());
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2());
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
	ImGuiChildFlags childFlags = ImGuiWindowFlags_NoDecoration;
	if (ImGui::BeginChild("##AmmoCountOverlay", {}, childFlags))
	{
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), std::format("{}/{}", Magazine.CurrentCartridges, Magazine.MaxCartridges).c_str());
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), Magazine.AmmoTypeName.c_str());
	}
	ImGui::EndChild();
	ImGui::PopStyleVar(6);
	ImGui::PopFont();
}