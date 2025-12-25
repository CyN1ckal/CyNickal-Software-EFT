#pragma once
#include "DMA/DMA.h"
#include "Game/Classes/Vector.h"
#include "Game/Classes/CCamera/CCamera.h"

class CameraList
{
public:
	static bool Initialize(DMA_Connection* Conn);
	static void QuickUpdateNecessaryCameras(DMA_Connection* Conn);
	static bool FPSCamera_W2S(const Vector3 WorldPosition, Vector2& ScreenPosition);

private:
	static inline std::vector<CCamera> m_CameraCache{};
	static inline CCamera* m_pFPSCamera{ nullptr };

private:
	static bool WorldToScreen(const Vector3 WorldPosition, Vector2& ScreenPosition, const Matrix44& CurMatrix);
	static bool CreateCameraCache(DMA_Connection* Conn, uintptr_t CameraHeadAddress, uint32_t NumCameras);
	static CCamera* SearchCameraCacheByName(const std::string& Name);
};