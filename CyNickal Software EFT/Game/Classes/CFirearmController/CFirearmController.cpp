/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CFirearmController.h"
#include "Game/Offsets/Offsets.h"

CFirearmController::CFirearmController(uintptr_t FirearmControllerAddress) : CHandsController(FirearmControllerAddress) {
	std::println("[CFirearmController] Constructed CFirearmController with 0x{:X}", m_EntityAddress);
}

void CFirearmController::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh, EPlayerType PlayerType) {
	CHandsController::PrepareRead_1(vmsh, PlayerType);

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CFirearmController::pFireport, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_FireportAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CFirearmController::LastShotId, sizeof(uint8_t), reinterpret_cast<BYTE*>(&m_ShotIndex), nullptr);
}

void CFirearmController::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	CHandsController::PrepareRead_2(vmsh);

	if (!m_FireportAddress) {
		std::println("[CFirearmController] Fireport address is null, marking firearm controller as invalid.");
		SetInvalid();
	}

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_FireportAddress + 0x10, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_FirstAddr), nullptr);
}

void CFirearmController::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	CHandsController::PrepareRead_3(vmsh);

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_FirstAddr + 0x10, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_FireportTransformAddr), nullptr);
}

void CFirearmController::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	CHandsController::PrepareRead_4(vmsh);

	if (!m_FireportTransformAddr)
		SetInvalid();

	if (IsInvalid()) return;

	m_FireportTransform.emplace(m_FireportTransformAddr, EAllocationType::EMPTY);
	m_FireportTransform->PrepareRead_1(vmsh);
}

void CFirearmController::PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh)
{
	CHandsController::PrepareRead_5(vmsh);

	if (IsInvalid()) return;

	m_FireportTransform->PrepareRead_2(vmsh);
}

void CFirearmController::PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh)
{
	CHandsController::PrepareRead_6(vmsh);

	if (IsInvalid()) return;

	m_FireportTransform->PrepareRead_3(vmsh);
}

void CFirearmController::PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh)
{
	CHandsController::PrepareRead_7(vmsh);

	if (IsInvalid()) return;

	m_FireportTransform->PrepareRead_4(vmsh);
}

void CFirearmController::QuickRead(VMMDLL_SCATTER_HANDLE vmsh) {
	CHandsController::QuickRead(vmsh, EPlayerType::eMainPlayer);

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CFirearmController::LastShotId, sizeof(uint8_t), reinterpret_cast<BYTE*>(&m_ShotIndex), nullptr);

	if (m_FireportTransform)
		m_FireportTransform->QuickRead(vmsh);
}

void CFirearmController::QuickFinalize() {
	CHandsController::QuickFinalize();

	if (!m_FireportTransform)
		SetInvalid();

	if (IsInvalid()) return;

	m_FireportTransform->QuickFinalize();
	m_FireportPosition = m_FireportTransform->GetPosition();
}

void CFirearmController::Finalize() {
	CHandsController::Finalize();

	if (IsInvalid()) return;

	m_FireportPosition = m_FireportTransform->GetPosition();
}