/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once

namespace NearbyItemsOverlay {
	void Render();
	inline int32_t m_MinimumItemValue{ 30000 };
	inline float m_fDistance{ 30.0f };
	inline bool bMasterToggle{ true };
}