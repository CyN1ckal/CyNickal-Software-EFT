/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "DMA/CDMAConnection/CDMAConnection.h"
#include "Game/Classes/Vector.h"
#include "Game/Classes/CCamera/CCamera.h"

class CameraList
{
public:
	static bool CompleteUpdate(CDMAConnection* Conn);
	static bool W2S(const Vector3 WorldPosition, Vector2& ScreenPosition);
	static bool OpticW2S(const Vector3 WorldPosition, Vector2& ScreenPosition);
	static CCamera* GetSelectedOptic();

	static void QuickUpdateNecessaryCameras(CDMAConnection* Conn);
	static inline uint32_t m_OpticIndex{ 0 };
	static float GetOpticRadius();
	static Vector2 GetOpticCenter();
	static void SetOpticRadius(float Width);

private:
	static inline std::mutex m_CamCacheLock{};
	static inline std::vector<CCamera> m_CameraCache{};
	static inline std::vector<CCamera*> m_pOpticCameras{};
	static inline CCamera* m_pFPSCamera{ nullptr };

private:
	static bool WorldToScreenEx(const Vector3 WorldPosition, Vector2& ScreenPosition, CCamera* FPSCamera, CCamera* OpticCamera = nullptr);
	static bool CreateCameraCache(CDMAConnection* Conn, uintptr_t CameraHeadAddress, uint32_t NumCameras);
	static CCamera* SearchCameraCacheByName(const std::string& Name);
	static std::vector<CCamera*> GetPotentialOpticCameras();
	static CCamera* FindWinningOptic(const std::vector<CCamera*>& PotentialOpticCams);
};