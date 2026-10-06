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
#include "DMA/CDMAConnection/CDMAConnection.h"
#include "Game/Classes/Vector.h"

class CCamera : public CBaseEntity
{
public:
	CCamera(uintptr_t CameraAddress);
	CCamera(CCamera&& Cam) noexcept;

public:
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	void QuickFinalize();

public:
	void FullUpdate(CDMAConnection* Conn);

private:
	uintptr_t m_GameObjectAddress{ 0 };
	uintptr_t m_ComponentsAddress{ 0 };
	uintptr_t m_CameraInfoAddress{ 0 };
	uintptr_t m_NameAddress{ 0 };
	std::array<char, 64> m_NameBuffer{};


	std::mutex m_FOVMutex{};
	float m_FOV{ 0.0f };
	float m_PrivateFOV{ 0.0f };

	std::mutex m_AspectRatioMutex{};
	float m_AspectRatio{ 0.0f };
	float m_PrivateAspectRatio{ 0.0f };

	std::mutex m_MatrixMutex{};
	Matrix44 m_PrivateViewMatrix{ 0 };
	Matrix44 m_ViewMatrix{ 0 };

private:
	inline void SetViewMatrix(const Matrix44& Mat);
	inline void SetFOV(const float& FOV);
	inline void SetAspectRatio(const float& AspectRatio);

public:
	Matrix44 GetViewMatrix();
	const float GetFOV();
	const float GetAspectRatio();
	const std::string_view GetName() const;
};