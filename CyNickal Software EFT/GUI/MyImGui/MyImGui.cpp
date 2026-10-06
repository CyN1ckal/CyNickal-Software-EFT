/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "MyImGui.h"

ImVec2 MyImGui::DrawTwoToneNameplate(CTwoToneNameplateInfo& Info, const Vector2& ScreenPos, ENamePlateAlignment Alignment)
{
	constexpr float PrefixNameSpacing = 2.0f;
	constexpr float HalfPrefixNameSpacing = PrefixNameSpacing * 0.5f;
	constexpr float OuterSpacing{ 1.0f };
	constexpr float TwoOuterSpacing{ OuterSpacing * 2.0f };

	const auto PrefixSize = ImGui::CalcTextSize(Info.Prefix.c_str());
	const auto NameSize = ImGui::CalcTextSize(Info.Name.c_str());
	const auto TextWidth = PrefixSize.x + NameSize.x + PrefixNameSpacing + TwoOuterSpacing;
	const auto HalfTextWidth = TextWidth * 0.5f;
	const auto LineHeight = ImGui::GetTextLineHeight();
	const auto WindowPos = ImGui::GetWindowPos();
	const auto DrawList = ImGui::GetWindowDrawList();

	float BaseX{ 0.0f };
	float BaseY{ 0.0f };

	if (Alignment == ENamePlateAlignment::Centered) {
		BaseX = std::roundf(ScreenPos.x - HalfTextWidth + WindowPos.x);
		BaseY = std::roundf(ScreenPos.y + WindowPos.y);
	}
	else if (Alignment == ENamePlateAlignment::LeftAligned) {
		BaseX = std::roundf(ScreenPos.x + WindowPos.x);
		BaseY = std::roundf(ScreenPos.y + WindowPos.y - (LineHeight * 0.5f));
	}

	const ImVec2 BaseTopLeft{ BaseX, BaseY };

	ImVec2 PrefixTopLeft{ BaseTopLeft };
	ImVec2 PrefixBottomRight{ BaseTopLeft.x + std::roundf(PrefixSize.x + OuterSpacing + HalfPrefixNameSpacing), BaseTopLeft.y + std::roundf(LineHeight) };
	DrawList->AddRectFilled(PrefixTopLeft, PrefixBottomRight, Info.PrefixBackgroundColor);

	ImVec2 NameTopLeft{ PrefixBottomRight.x, BaseTopLeft.y };
	ImVec2 NameBottomRight{ PrefixBottomRight.x + std::roundf(NameSize.x + OuterSpacing + HalfPrefixNameSpacing), BaseTopLeft.y + std::roundf(LineHeight) };
	DrawList->AddRectFilled(NameTopLeft, NameBottomRight, Info.NameBackgroundColor);

	ImVec2 TextPos{ BaseTopLeft.x + OuterSpacing, BaseTopLeft.y - OuterSpacing };
	DrawList->AddText(TextPos, Info.ProfileTextColor, Info.Prefix.c_str());

	TextPos.x += std::roundf(PrefixSize.x + PrefixNameSpacing);
	DrawList->AddText(TextPos, Info.NameTextColor, Info.Name.c_str());

	return ImVec2(TextWidth, LineHeight);
}