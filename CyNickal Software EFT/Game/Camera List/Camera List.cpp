#include "pch.h"
#include "Camera List.h"
#include "Game/EFT.h"
#include "Game/Offsets/Offsets.h"
#include "GUI/Fuser/Fuser.h"

Matrix44 TransposeMatrix(const Matrix44& Mat)
{
	Matrix44 Result{};

	Result.M[0][0] = Mat.M[0][0];
	Result.M[0][1] = Mat.M[1][0];
	Result.M[0][2] = Mat.M[2][0];
	Result.M[0][3] = Mat.M[3][0];

	Result.M[1][0] = Mat.M[0][1];
	Result.M[1][1] = Mat.M[1][1];
	Result.M[1][2] = Mat.M[2][1];
	Result.M[1][3] = Mat.M[3][1];

	Result.M[2][0] = Mat.M[0][2];
	Result.M[2][1] = Mat.M[1][2];
	Result.M[2][2] = Mat.M[2][2];
	Result.M[2][3] = Mat.M[3][2];

	Result.M[3][0] = Mat.M[0][3];
	Result.M[3][1] = Mat.M[1][3];
	Result.M[3][2] = Mat.M[2][3];
	Result.M[3][3] = Mat.M[3][3];

	return Result;
}

float DotProduct(const Vector3& a, const Vector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

bool CameraList::WorldToScreen(const Vector3 WorldPosition, Vector2& ScreenPosition, const Matrix44& CurMatrix)
{
	auto Transposed = TransposeMatrix(CurMatrix);
	Vector3 TranslationVector{ Transposed.M[3][0],Transposed.M[3][1],Transposed.M[3][2] };
	Vector3 Up{ Transposed.M[1][0],Transposed.M[1][1],Transposed.M[1][2] };
	Vector3 Right{ Transposed.M[0][0],Transposed.M[0][1],Transposed.M[0][2] };
	Vector3 vec3{};
	float w = DotProduct(TranslationVector, WorldPosition) + Transposed.M[3][3];

	if (w < 0.098f)
		return false;

	float y = DotProduct(WorldPosition, Up) + Transposed.M[1][3];
	float x = DotProduct(WorldPosition, Right) + Transposed.M[0][3];

	auto CenterScreen = Fuser::GetCenterScreen();

	ScreenPosition.x = (CenterScreen.x) * (1.f + x / w);
	ScreenPosition.y = (CenterScreen.y) * (1.f - y / w);

	return true;
}

bool CameraList::FPSCamera_W2S(const Vector3 WorldPosition, Vector2& ScreenPosition)
{
	if (auto FPSCam = SearchCameraCacheByName("FPS Camera"))
		return WorldToScreen(WorldPosition, ScreenPosition, FPSCam->GetViewMatrix());

	return false;
}

bool CameraList::Initialize(DMA_Connection* Conn)
{
	auto& Proc = EFT::GetProcess();

	auto CCamerasAddress = Proc.ReadMem<uintptr_t>(Conn, EFT::GetProcess().GetUnityAddress() + Offsets::pCameras);
	auto CameraHeadAddress = Proc.ReadMem<uintptr_t>(Conn, CCamerasAddress + Offsets::CCameras::pCameraList);
	auto NumCameras = Proc.ReadMem<uint32_t>(Conn, CCamerasAddress + Offsets::CCameras::NumCameras);

	if (NumCameras > 128 || NumCameras == 0)
		throw std::runtime_error("Invalid number of cameras: " + std::to_string(NumCameras));

	CreateCameraCache(Conn, CameraHeadAddress, NumCameras);

	auto FPSCam = SearchCameraCacheByName("FPS Camera");

	if (!FPSCam)
	{
		std::println("[Camera] Failed to find FPS Camera in cache.");
		return false;
	}

	return false;
}

bool CameraList::CreateCameraCache(DMA_Connection* Conn, uintptr_t CameraHeadAddress, uint32_t NumCameras)
{
	auto& Proc = EFT::GetProcess();

	m_CameraCache.clear();
	m_CameraCache.reserve(NumCameras);

	std::println("[Camera] Creating camera cache with {} cameras from {:X}", NumCameras, CameraHeadAddress);
	std::vector<uintptr_t> CameraAddresses = Proc.ReadVec<uintptr_t>(Conn, CameraHeadAddress, NumCameras);

	for (auto Addr : CameraAddresses)
	{
		if (!Addr)
			continue;

		m_CameraCache.emplace_back(Addr);
		m_CameraCache.back().FullUpdate(Conn);
	}

	std::println("[Camera] Cached {} cameras.", m_CameraCache.size());

	if (auto FPSCam = SearchCameraCacheByName("FPS Camera"))
		m_pFPSCamera = FPSCam;
	else
		std::println("[Camera] Failed to find FPS Camera in cache.");

	return true;
}

CCamera* CameraList::SearchCameraCacheByName(const std::string& Name)
{
	for (auto& Entry : m_CameraCache)
	{
		if (Entry.GetName() == Name.c_str())
			return &Entry;
	}

	return nullptr;
}

void CameraList::QuickUpdateNecessaryCameras(DMA_Connection* Conn)
{
	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), EFT::GetProcess().GetPID(), VMMDLL_FLAG_NOCACHE);

	if (m_pFPSCamera)
		m_pFPSCamera->QuickRead(vmsh);

	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	if (m_pFPSCamera)
		m_pFPSCamera->QuickFinalize();
}