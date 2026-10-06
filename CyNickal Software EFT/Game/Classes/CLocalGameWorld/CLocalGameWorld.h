/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Classes/CLootList/CLootList.h"
#include "Game/Classes/CRegisteredPlayers/CRegisteredPlayers.h"
#include "Game/Classes/CExfilController/CExfilController.h"
#include "Game/Classes/CGrenades/CGrenades.h"
#include "Game/Enums/EMap.h"
#include "GUI/Image Loader/Image Loader.h"

class CLocalGameWorld : public CBaseEntity
{
public:
	CLocalGameWorld(uintptr_t GameWorldAddress);

public:
	void QuickUpdateItems(CDMAConnection* Conn);
	void QuickUpdatePlayers(CDMAConnection* Conn);
	void QuickUpdateGrenades(CDMAConnection* Conn);
	void HandlePlayerAllocations(CDMAConnection* Conn);
	void HandleLootListAllocations(CDMAConnection* Conn);
	void FullUpdateGrenades(CDMAConnection* Conn);

public:
	bool IsValidRaid(CDMAConnection* Conn);
	uintptr_t GetMainPlayerAddress() const { return m_MainPlayerAddress; }
	void SetNeedsRefresh() { m_Flags |= 0x2; }
	const bool NeedsRefresh() const { return (m_Flags & 0x2); }

private:
	CTextureInfo GetCurrentMapTex() const;

public:
	std::unique_ptr<class CLootList> m_pLootList{ nullptr };
	std::unique_ptr<class CRegisteredPlayers> m_pRegisteredPlayers{ nullptr };
	std::unique_ptr<class CExfilController> m_pExfilController{ nullptr };
	std::unique_ptr<class CGrenades> m_pGrenades{ nullptr };
	EMap m_CurrentMap{ EMap::UNKNOWN };
	CTextureInfo m_MapTexture{};

private:
	std::uintptr_t m_MainPlayerAddress{};
	std::uintptr_t LootListAddress{};
	std::uintptr_t RegisteredPlayersAddress{};
	std::uintptr_t ExfiltrationControllerAddress{};
};