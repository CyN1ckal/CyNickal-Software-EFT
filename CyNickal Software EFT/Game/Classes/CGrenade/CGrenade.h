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
#include "Game/Classes/CUnityTransform/CUnityTransform.h"

class CGrenade : public CBaseEntity {
public:
	CGrenade(uintptr_t GrenadeAddress);

	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_9(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_10(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickFinalize();

public:
	Vector3 m_LastPosition{};

private:
	std::optional<CUnityTransform> m_Transform{ std::nullopt };
	uintptr_t TransformChain_1{ 0 };
	uintptr_t m_GameObjectAddress{ 0 };
	uintptr_t m_GameObjectComponentsAddress{ 0 };
	uintptr_t m_ComponentTransforms{ 0 };
	uintptr_t m_ObjectClassAddress{ 0 };
	uintptr_t m_UnityTransformAddress{ 0 };
};