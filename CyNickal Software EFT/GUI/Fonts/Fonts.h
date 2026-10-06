/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once

class Fonts
{
public:
	static void Initialize(ImGuiIO& io);

public:
	static inline ImFont* m_IBMPlexMonoSemiBold{ nullptr };
};