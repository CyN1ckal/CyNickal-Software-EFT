/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "GUI/Image Loader/Image Loader.h"
#include "Game/Classes/Vector.h"

struct CTwoToneNameplateInfo
{
	std::string Prefix{};
	std::string Name{};
	ImColor PrefixBackgroundColor{ 255,255,255,255 };
	ImColor NameBackgroundColor{ 255,255,255,255 };
	ImColor ProfileTextColor{ 255,255,255,255 };
	ImColor NameTextColor{ 255,255,255,255 };
};

enum class ENamePlateAlignment {
	Centered = 0,
	LeftAligned = 1
};

namespace MyImGui {
	ImVec2 DrawTwoToneNameplate(CTwoToneNameplateInfo& Info, const Vector2& ScreenPos, ENamePlateAlignment Alignment = ENamePlateAlignment::Centered);
}