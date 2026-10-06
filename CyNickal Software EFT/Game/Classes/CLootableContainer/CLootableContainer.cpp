/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CLootableContainer.h"
#include "Game/Offsets/Offsets.h"
#include "Database/Database.h"
#include "GUI/Windows/Color Picker/Color Picker.h"

CLootableContainer::CLootableContainer(uintptr_t EntityAddress) : CBaseLootItem(EntityAddress)
{
	//std::println("[CLootableContainer] Constructed with {0:X}", m_EntityAddress);
}

void CLootableContainer::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseLootItem::PrepareRead_1(vmsh);

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CLootableContainer::pBSGID, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_TarkovIDAddressPointer), nullptr);
}

void CLootableContainer::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseLootItem::PrepareRead_2(vmsh);

	if (!m_TarkovIDAddressPointer)
		SetInvalid();

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_TarkovIDAddressPointer + 0x14, sizeof(m_TarkovID), reinterpret_cast<BYTE*>(m_TarkovID.data()), nullptr);
}

void CLootableContainer::Finalize()
{
	CBaseLootItem::Finalize();

	if (IsInvalid()) return;

	std::string BSGID = std::string(m_TarkovID.begin(), m_TarkovID.end());

	m_Name = TarkovContainerData::GetNameOfContainer(BSGID);
}

const ImColor& CLootableContainer::GetRadarColor() const
{
	return ColorPicker::Radar::m_ContainerColor;
}

const ImColor& CLootableContainer::GetFuserColor() const
{
	return ColorPicker::Fuser::m_ContainerColor;
}