/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CLocalGameWorld.h"
#include "Game/Offsets/Offsets.h"
#include "DMA/CDMAConnection/CDMAConnection.h"
#include "Game/EFT.h"
#include "Game/Quest Manager/Quest Manager.h"
#include "Game/Camera List/Camera List.h"
#include "GUI/Texture Manager/Texture Manager.h"

EMap DetermineMapEnum(const std::wstring& mapName);

std::unordered_map<EMap, std::string> MapFilenames{
	{ EMap::FACTORY, "Factory.png" },
	{ EMap::CUSTOMS, "Customs.png" },
	{ EMap::INTERCHANGE, "Interchange.png" },
	{ EMap::WOODS, "Woods.png" },
	{ EMap::RESERVE, "Reserve.png" },
	{ EMap::SHORELINE, "Shoreline.png" },
	{ EMap::GROUND_ZERO, "GroundZero.png" },
	{ EMap::STREETS, "Streets.png" },
};

std::unordered_map<EMap, CTextureInfo> MapTextureCache{};

CLocalGameWorld::CLocalGameWorld(uintptr_t GameWorldAddress) : CBaseEntity(GameWorldAddress)
{
	ZoneScoped;

	std::println("[CLocalGameWorld] Created CLocalGameWorld at address: 0x{:X}", GameWorldAddress);

	try {
		auto Conn = CDMAConnection::GetInstance();
		auto& Proc = EFT::GetProcess();

		m_CurrentMap = DetermineMapEnum(Proc.ReadUnityStringPtr<wchar_t>(Conn, GameWorldAddress + Offsets::CLocalGameWorld::pMapName));

		m_MapTexture = GetCurrentMapTex();

		m_MainPlayerAddress = Proc.ReadMem<uintptr_t>(Conn, GameWorldAddress + Offsets::CLocalGameWorld::pMainPlayer);

		RegisteredPlayersAddress = Proc.ReadMem<uintptr_t>(Conn, GameWorldAddress + Offsets::CLocalGameWorld::pRegisteredPlayers);
		m_pRegisteredPlayers = std::make_unique<CRegisteredPlayers>(RegisteredPlayersAddress);
		CLocalGameWorld::HandlePlayerAllocations(Conn);

		QuestManager::CompleteUpdate(Conn, m_MainPlayerAddress);

		LootListAddress = Proc.ReadMem<uintptr_t>(Conn, GameWorldAddress + Offsets::CLocalGameWorld::pLootList);
		m_pLootList = std::make_unique<CLootList>(LootListAddress);

		ExfiltrationControllerAddress = Proc.ReadMem<uintptr_t>(Conn, GameWorldAddress + Offsets::CLocalGameWorld::pExfiltrationController);
		m_pExfilController = std::make_unique<CExfilController>(ExfiltrationControllerAddress, m_CurrentMap);

		uintptr_t GrenadesAddress = Proc.ReadMem<uintptr_t>(Conn, GameWorldAddress + Offsets::CLocalGameWorld::pGrenades);
		m_pGrenades = std::make_unique<CGrenades>(GrenadesAddress);

		CameraList::CompleteUpdate(Conn);
	}
	catch (const std::exception& e) {
		std::println("[CLocalGameWorld] Exception during initialization: {}", e.what());
		SetInvalid();
	}
}

void CLocalGameWorld::QuickUpdateItems(CDMAConnection* Conn) {
	if (m_pLootList)
		m_pLootList->QuickUpdate(Conn);
}

void CLocalGameWorld::QuickUpdatePlayers(CDMAConnection* Conn)
{
	if (m_pRegisteredPlayers)
		m_pRegisteredPlayers->QuickUpdate(Conn);
}

void CLocalGameWorld::QuickUpdateGrenades(CDMAConnection* Conn)
{
	if (m_pGrenades)
		m_pGrenades->QuickUpdate(Conn);
}

void CLocalGameWorld::HandlePlayerAllocations(CDMAConnection* Conn)
{
	if (m_pRegisteredPlayers == nullptr) return;

	ZoneScoped;

	m_pRegisteredPlayers->UpdateBaseAddresses(Conn);
	m_pRegisteredPlayers->HandlePlayerAllocations(Conn, m_MainPlayerAddress);
}

void CLocalGameWorld::HandleLootListAllocations(CDMAConnection* Conn)
{
	if (m_pLootList == nullptr) return;

	ZoneScoped;

	m_pLootList->CompleteUpdate(Conn);
}

void CLocalGameWorld::FullUpdateGrenades(CDMAConnection* Conn)
{
	if (m_pGrenades)
		m_pGrenades->CompleteUpdate(Conn);
}

bool CLocalGameWorld::IsValidRaid(CDMAConnection* Conn)
{
	if (IsInvalid()) return false;

	if (m_pExfilController == nullptr || m_pExfilController->IsInvalid()) {
		std::println("[CLocalGameWorld] Invalid Exfiltration Controller");
		return false;
	}

	if (m_pRegisteredPlayers == nullptr || m_pRegisteredPlayers->IsInvalid()) {
		std::println("[CLocalGameWorld] Invalid Registered Players");
		return false;
	}

	if (m_pLootList == nullptr || m_pLootList->IsInvalid()) {
		std::println("[CLocalGameWorld] Invalid Loot List");
		return false;
	}

	if (m_pRegisteredPlayers->GetNumValidPlayers() == 0) {
		std::println("[CLocalGameWorld] No valid players found in raid");
		return false;
	}

	return true;
}

CTextureInfo CLocalGameWorld::GetCurrentMapTex() const {
	if (MapFilenames.find(m_CurrentMap) == MapFilenames.end())
		return ResourceManager::GetMissingTexture();

	return ResourceManager::GetTexture(MapFilenames.at(m_CurrentMap));
}

EMap DetermineMapEnum(const std::wstring& mapName)
{
	if (mapName.size() < 5) return EMap::UNKNOWN;

	if (mapName[0] == 'f')
		return EMap::FACTORY;

	if (mapName[0] == 'W')
		return EMap::WOODS;

	if (mapName[0] == 'b')
		return EMap::CUSTOMS;

	if (mapName[0] == 'R')
		return EMap::RESERVE;

	if (mapName[0] == 'I')
		return EMap::INTERCHANGE;

	if (mapName[0] == 'l')
		return EMap::LABS;

	if (mapName[0] == 'L') {
		if (mapName[1] == 'i') {
			return EMap::LIGHTHOUSE;
		}
		else {
			return EMap::LABYRINTH;
		}
	}

	if (mapName[0] == 'T') {
		if (mapName[1] == 'a') {
			return EMap::STREETS;
		}
		else
			return EMap::TERMINAL;
	}

	if (mapName[0] == 'S') {

		if (mapName[1] == 'a') {
			return EMap::GROUND_ZERO;
		}
		else if (mapName[1] == 'h') {
			return EMap::SHORELINE;
		}
	}

	return EMap::UNKNOWN;
}