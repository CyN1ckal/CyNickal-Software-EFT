/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "EFT.h"
#include "Game/GOM/GOM.h"
#include "Game/Response Data/Response Data.h"
#include "GUI/Windows/Flea Bot/Flea Bot.h"
#include "Game/Offsets/Offsets.h"

bool EFT::Initialize(CDMAConnection* Conn)
{
	std::println("[EFT] Initializing EFT module...");

	Proc.GetProcessInfo(Conn);

	Offsets::ResolveAll(Proc);

	ResponseData::Initialize(Conn);

	CreateWorldIfNeeded(Conn);

	return true;
}

const CProcess& EFT::GetProcess()
{
	return Proc;
}

void EFT::CreateWorldIfNeeded(CDMAConnection* Conn)
{
	if (FleaBot::bMasterToggle) {return;}

	if (pGameWorld && pGameWorld->NeedsRefresh()) {
		ZoneScopedN("EFT::CreateWorldIfNeeded::Refresh");
		std::println("[EFT] Current raid needs refresh...");
		std::scoped_lock Lock(m_GameWorldMutex);
		pGameWorld = std::make_unique<CLocalGameWorld>(pGameWorld->m_EntityAddress);
		return;
	}

	if (pGameWorld && pGameWorld->IsValidRaid(Conn)) {
		return;
	}

	ZoneScopedN("EFT::CreateWorldIfNeeded::Invalid");
	std::println("[EFT] Invalid raid detected.");
	auto LatestWorldAddr = GOM::GetLatestWorldAddr(Conn);
	std::scoped_lock Lock(m_GameWorldMutex);
	pGameWorld.reset();
	if (LatestWorldAddr) {
		pGameWorld = std::make_unique<CLocalGameWorld>(LatestWorldAddr);
	}
}

uintptr_t EFT::GetMainPlayerAddress()
{
	if (pGameWorld)
		return pGameWorld->GetMainPlayerAddress();

	return uintptr_t();
}

void EFT::QuickUpdatePlayers(CDMAConnection* Conn)
{
	if (pGameWorld)
		pGameWorld->QuickUpdatePlayers(Conn);
}

void EFT::QuickUpdateGrenades(CDMAConnection* Conn)
{
	if (pGameWorld)
		pGameWorld->QuickUpdateGrenades(Conn);
}

void EFT::QuickUpdateItems(CDMAConnection* Conn)
{
	if (pGameWorld)
		pGameWorld->QuickUpdateItems(Conn);
}

void EFT::HandlePlayerAllocations(CDMAConnection* Conn)
{
	if (pGameWorld)
		pGameWorld->HandlePlayerAllocations(Conn);
}

void EFT::HandleLootListAllocations(CDMAConnection* Conn)
{
	if (pGameWorld)
		pGameWorld->HandleLootListAllocations(Conn);
}

void EFT::FullUpdateGrenades(CDMAConnection* Conn)
{
	if (pGameWorld)
		pGameWorld->FullUpdateGrenades(Conn);
}

CRegisteredPlayers& EFT::GetRegisteredPlayers()
{
	if (!pGameWorld)
		throw std::runtime_error("EFT::pGameWorld is null");

	if (!pGameWorld->m_pRegisteredPlayers)
		throw std::runtime_error("EFT::pGameWorld->m_pRegisteredPlayers is null");

	return *(pGameWorld->m_pRegisteredPlayers);
}

CLootList& EFT::GetLootList()
{
	if (!pGameWorld)
		throw std::runtime_error("EFT::pGameWorld is null");

	if (!pGameWorld->m_pLootList)
		throw std::runtime_error("EFT::pGameWorld->m_pLootList is null");

	return *(pGameWorld->m_pLootList);
}

CExfilController& EFT::GetExfilController()
{
	if (!pGameWorld)
		throw std::runtime_error("EFT::pGameWorld is null");

	if (!pGameWorld->m_pExfilController)
		throw std::runtime_error("EFT::pGameWorld->m_pRegisteredExfils is null");

	return *(pGameWorld->m_pExfilController);
}

EMap EFT::GetCurrentMap()
{
	std::scoped_lock Lock(m_GameWorldMutex);
	if (pGameWorld)
		return pGameWorld->m_CurrentMap;

	return EMap::UNKNOWN;
}