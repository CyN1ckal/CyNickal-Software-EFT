/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once

namespace ColorPicker
{
	void Render();
	void MyColorPicker(const char* label, ImColor& color);

	inline bool bMasterToggle{ false };

	namespace Fuser
	{
		void Render();
		inline ImColor m_PMCColor{ ImColor(200,0,0) };
		inline ImColor m_ScavColor{ ImColor(200,200,0, 200) };
		inline ImColor m_PlayerScavColor{ ImColor(220,170,0) };
		inline ImColor m_BossColor{ ImColor(225,0,225) };
		inline ImColor m_LootColor{ ImColor(0,150,150) };
		inline ImColor m_ContainerColor{ ImColor(108,150,150) };
		inline ImColor m_ExfilColor{ ImColor(25,225,25, 200) };
		inline ImColor m_WeaponTextColor{ ImColor(255,255,255) };
		inline ImColor m_Objective{ ImColor(113,9,249) };
		inline ImColor m_QuestItems{ ImColor(252,213,85) };
		inline ImColor m_CorpseColor{ ImColor(255,255,255,66) };
		inline ImColor m_GrenadeColor{ ImColor(255,100,0) };
	}

	namespace Radar
	{
		void Render();
		inline ImColor m_PMCColor{ ImColor(200,0,0) };
		inline ImColor m_ScavColor{ ImColor(200,200,0) };
		inline ImColor m_PlayerScavColor{ ImColor(220,170,0) };
		inline ImColor m_LocalPlayerColor{ ImColor(0,200,0) };
		inline ImColor m_BossColor{ ImColor(225,0,225) };
		inline ImColor m_LootColor{ ImColor(0,150,150) };
		inline ImColor m_ContainerColor{ ImColor(108,150,150) };
		inline ImColor m_ExfilColor{ ImColor(25,225,25) };
		inline ImColor m_Objective{ ImColor(113,9,249) };
		inline ImColor m_QuestItems{ ImColor(252,213,85) };
		inline ImColor m_CorpseColor{ ImColor(255,255,255) };
		inline ImColor m_GrenadeColor{ ImColor(255,100,0) };
	}
};