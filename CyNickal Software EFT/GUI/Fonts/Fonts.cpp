/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Fonts.h"
#include "Data/IBMPlexMono.h"

void Fonts::Initialize(ImGuiIO& io)
{
	m_IBMPlexMonoSemiBold = io.Fonts->AddFontFromMemoryTTF(IBMPlexMonoSemiBoldData, sizeof(IBMPlexMonoSemiBoldData), 16.0f);
	IM_ASSERT(m_IBMPlexMonoSemiBold != nullptr);
}