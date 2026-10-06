/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "Game/Classes/CHandsController/CHandsController.h"
#include "Game/Classes/CUnityTransform/CUnityTransform.h"

class CFirearmController : public CHandsController {
public:
	CFirearmController(uintptr_t FirearmControllerAddress);

	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh, EPlayerType PlayerType);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickFinalize();
	void Finalize();

public:
	std::optional<CUnityTransform> m_FireportTransform{ std::nullopt };
	uintptr_t m_FireportAddress{ 0 };
	uintptr_t m_FirstAddr{ 0 };
	uintptr_t m_FireportTransformAddr{ 0 };
	Vector3 m_FireportPosition{};
	uint8_t m_ShotIndex{ 0 };
};