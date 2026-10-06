/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Ammo Count.h"

#include "Game/EFT.h"

void AmmoCountOverlay::Render()
{
	if (!bMasterToggle) return;

	ZoneScoped;

	auto WindowSize = ImGui::GetWindowSize();
	auto WindowPos = ImGui::GetWindowPos();

	auto Magazine = EFT::GetRegisteredPlayers().GetLocalPlayerMagazine();

	if (Magazine.m_MaxCartridges == 0) return;

	std::string AmmoString = std::format("{}/{}", Magazine.m_CurrentCartridges, Magazine.m_MaxCartridges);

	ImGui::PushFont(nullptr, 32.0f);
	auto AmmoTextSize = ImGui::CalcTextSize(AmmoString.c_str());
	auto TypeTextSize = ImGui::CalcTextSize(Magazine.m_AmmoTypeName.c_str());
	float MaxTextWidth = (AmmoTextSize.x > TypeTextSize.x) ? AmmoTextSize.x : TypeTextSize.x;

	constexpr float Margin = 64.0f;
	ImVec2 WindowPadding = { 8.0f, 8.0f };
	float ItemSpacingY = ImGui::GetStyle().ItemSpacing.y;
	ImVec2 ChildSize = {
		MaxTextWidth + WindowPadding.x * 2.0f,
		AmmoTextSize.y + TypeTextSize.y + ItemSpacingY + WindowPadding.y * 2.0f
	};

	ImGui::SetCursorPos({
		WindowSize.x - ChildSize.x - Margin,
		WindowSize.y - ChildSize.y - Margin
		});

	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, WindowPadding);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 0.85f));
	ImGuiChildFlags childFlags = ImGuiChildFlags_AlwaysUseWindowPadding;
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration;
	if (ImGui::BeginChild("##AmmoCountOverlay", ChildSize, childFlags, windowFlags))
	{
		float ContentWidth = ChildSize.x - WindowPadding.x * 2.0f;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ContentWidth - AmmoTextSize.x) * 0.5f);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", AmmoString.c_str());

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ContentWidth - TypeTextSize.x) * 0.5f);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", Magazine.m_AmmoTypeName.c_str());
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
	ImGui::PopFont();
}