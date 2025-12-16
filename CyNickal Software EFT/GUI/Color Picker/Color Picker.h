#pragma once

namespace ColorPicker
{
	void Render();
	void MyColorPicker(const char* label, ImColor& color);

	static inline bool bMasterToggle{ true };

	namespace Fuser
	{
		void Render();
		inline ImColor m_PMCColor{ ImColor(200,0,0) };
		inline ImColor m_ScavColor{ ImColor(200,200,0) };
		inline ImColor m_PlayerScavColor{ ImColor(220,170,0) };
		inline ImColor m_BossColor{ ImColor(225,0,225) };
		inline ImColor m_LootColor{ ImColor(0,150,150) };
		inline ImColor m_ContainerColor{ ImColor(108,150,150) };
		inline ImColor m_ExfilColor{ ImColor(25,225,25) };
		inline ImColor m_WeaponTextColor{ ImColor(255,255,255) };
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
	}
};