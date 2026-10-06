/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CGrenades.h"
#include "Game/Offsets/Offsets.h"
#include "Game/Classes/CUnityList/CUnityList.hpp"

CUnityList<uintptr_t> GetGrenadeAddresses(CDMAConnection* Conn, uintptr_t EntAddr)
{
	ZoneScoped;

	CUnityList<uintptr_t> GrenadeList(Conn, EntAddr + Offsets::CGrenades::pGrenadeList);

	return GrenadeList;
}

CGrenades::CGrenades(uintptr_t GrenadesAddress) : CBaseEntity(GrenadesAddress) {
	std::println("[CGrenades] Constructed with 0x{:X}", GrenadesAddress);
}

void CGrenades::CompleteUpdate(CDMAConnection* Conn) {
	ZoneScoped;

	std::scoped_lock Lock(m_Grenades.m_Mutex);

	m_Grenades.clear();

	auto Grenades = GetGrenadeAddresses(CDMAConnection::GetInstance(), m_EntityAddress);

	m_Grenades.HandleAllocations(Grenades.m_Entries);

	auto PID = EFT::GetProcess().GetPID();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), PID, VMMDLL_FLAG_NOCACHE);
	m_Grenades.ForEach([&](CGrenade& Grenade) {	Grenade.PrepareRead_1(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);
	m_Grenades.ForEach([&](CGrenade& Grenade) {	Grenade.PrepareRead_2(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);
	m_Grenades.ForEach([&](CGrenade& Grenade) {	Grenade.PrepareRead_3(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);
	m_Grenades.ForEach([&](CGrenade& Grenade) {	Grenade.PrepareRead_4(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);
	m_Grenades.ForEach([&](CGrenade& Grenade) {	Grenade.PrepareRead_5(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);
	m_Grenades.ForEach([&](CGrenade& Grenade) {	Grenade.PrepareRead_6(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);
	m_Grenades.ForEach([&](CGrenade& Grenade) {	Grenade.PrepareRead_7(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);
	m_Grenades.ForEach([&](CGrenade& Grenade) {	Grenade.PrepareRead_8(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);
	m_Grenades.ForEach([&](CGrenade& Grenade) {	Grenade.PrepareRead_9(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);
	m_Grenades.ForEach([&](CGrenade& Grenade) {	Grenade.PrepareRead_10(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);
	m_Grenades.ForEach([](CGrenade& Grenade) {	Grenade.Finalize(); });
}

void CGrenades::QuickUpdate(CDMAConnection* Conn)
{
	ZoneScoped;

	std::scoped_lock Lock(m_Grenades.m_Mutex);

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), EFT::GetProcess().GetPID(), VMMDLL_FLAG_NOCACHE);
	m_Grenades.ForEach([&](CGrenade& Grenade) {	Grenade.QuickRead(vmsh); });
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	m_Grenades.ForEach([](CGrenade& Grenade) {	Grenade.QuickFinalize(); });
}
