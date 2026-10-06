/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CGrenade.h"
#include "Game/Offsets/Offsets.h"

CGrenade::CGrenade(uintptr_t GrenadeAddress) : CBaseEntity(GrenadeAddress) {
	//std::println("[CGrenade] Constructed with 0x{:X}", GrenadeAddress);
}

void CGrenade::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + 0x10, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&TransformChain_1), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CGrenade::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, TransformChain_1 + Offsets::CComponent::pGameObject, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_GameObjectAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CGrenade::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_GameObjectAddress + Offsets::CGameObject::pComponents, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_GameObjectComponentsAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CGrenade::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_GameObjectComponentsAddress + Offsets::CComponents::pTransform, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ComponentTransforms), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CGrenade::PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_ComponentTransforms + Offsets::CComponent::pObjectClass, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ObjectClassAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CGrenade::PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_ObjectClassAddress + 0x10, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_UnityTransformAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CGrenade::PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	m_Transform = CUnityTransform(m_UnityTransformAddress);
	m_Transform->PrepareRead_1(vmsh);
}

void CGrenade::PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_Transform->IsInvalid())
		SetInvalid();

	if (IsInvalid())
		return;

	m_Transform->PrepareRead_2(vmsh);
}

void CGrenade::PrepareRead_9(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_Transform->IsInvalid())
		SetInvalid();

	if (IsInvalid())
		return;

	m_Transform->PrepareRead_3(vmsh);
}

void CGrenade::PrepareRead_10(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_Transform->IsInvalid())
		SetInvalid();

	if (IsInvalid())
		return;

	m_Transform->PrepareRead_4(vmsh);
}

void CGrenade::Finalize()
{
	if (IsInvalid())
		return;

	m_LastPosition = m_Transform->GetPosition();
}

void CGrenade::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid())
		return;

	if (m_Transform) {
		/*
			For some reason grenades arent happy with the usual QuickUpdate.
			Using PrepareRead_4 works fine.
		*/
		m_Transform->PrepareRead_4(vmsh);
	}
}

void CGrenade::QuickFinalize()
{
	if (IsInvalid())
		return;

	m_LastPosition = m_Transform->GetPosition();
}
