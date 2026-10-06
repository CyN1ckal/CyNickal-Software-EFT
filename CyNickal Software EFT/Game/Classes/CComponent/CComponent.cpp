/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CComponent.h"
#include "Game/Offsets/Offsets.h"
#include "Game/Constants/EngineConstants.h"

CComponent::CComponent(uintptr_t ComponentAddress) : CBaseEntity(ComponentAddress) {
}

void CComponent::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh) {
	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CComponent::pObjectClass, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ComponentClassAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CComponent::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh) {
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_ComponentClassAddress, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_TypeInfoAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CComponent::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh) {
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_TypeInfoAddress + Offsets::CComponentTypeInfo::pName, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_TypeInfoNameAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CComponent::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh) {
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	m_ComponentName.resize(Constants::OBJECT_NAME_LENGTH + 1);
	VMMDLL_Scatter_PrepareEx(vmsh, m_TypeInfoNameAddress, Constants::OBJECT_NAME_LENGTH, reinterpret_cast<BYTE*>(m_ComponentName.data()), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CComponent::Finalize() {
	if (m_BytesRead != Constants::OBJECT_NAME_LENGTH)
		SetInvalid();
}
