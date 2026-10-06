/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "Game/Classes/Vector.h"

class CRadarDrawable {
protected:
	ImVec2 m_ScreenPos{};
	ImColor m_PrimaryColor{};
	float m_Radius{ 5.0f };

public:
	static void SetLocalPlayerPos(const Vector3& LocalPlayerPos) { m_LocalPlayerPos = LocalPlayerPos; }
	static void SetCenterRadar(const ImVec2& CenterRadar) { m_CenterRadar = CenterRadar; }

private:
	static inline Vector3 m_LocalPlayerPos{};
	static inline ImVec2 m_CenterRadar{};

public:
	CRadarDrawable(const Vector3& WorldPos, const ImColor& PrimaryColor);
	void Draw(ImDrawList* DrawList) const;
};