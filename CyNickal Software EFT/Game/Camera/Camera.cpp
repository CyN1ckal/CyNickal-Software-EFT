#include "pch.h"
#include "Camera.h"
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

bool Camera::WorldToScreen(Vector3 WorldPosition, Vector2& ScreenPosition)
{
	auto CurMatrix = Camera::GetViewMatrix();
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

bool Camera::Initialize(DMA_Connection* Conn)
{
	auto& Proc = EFT::GetProcess();

	uintptr_t CCamerasAddress = Proc.ReadMem<uintptr_t>(Conn, EFT::GetProcess().GetUnityAddress() + Offsets::pCameras);

	m_CameraHeadAddress = Proc.ReadMem<uintptr_t>(Conn, CCamerasAddress + Offsets::CCameras::pCameraList);
	m_NumCameras = Proc.ReadMem<uint32_t>(Conn, CCamerasAddress + Offsets::CCameras::NumCameras);

	if (m_NumCameras > 128 || m_NumCameras == 0)
		throw std::runtime_error("Invalid number of cameras: " + std::to_string(m_NumCameras));

	InitializeCameraCache(Conn);

	auto FPSCamAddress = SearchCameraCacheByName("FPS Camera");

	if (!FPSCamAddress)
	{
		std::println("[Camera] Failed to find FPS Camera in cache.");
		return false;
	}

	std::println("[Camera] FPS Camera Address: 0x{:X}", FPSCamAddress);

	auto ObjectAddress = Proc.ReadMem<uintptr_t>(Conn, FPSCamAddress + Offsets::CComponent::pGameObject);
	auto ComponentAddress = Proc.ReadMem<uintptr_t>(Conn, ObjectAddress + Offsets::CGameObject::pComponents);
	auto CameraInfoAddress = Proc.ReadMem<uintptr_t>(Conn, ComponentAddress + Offsets::CCamera::pCameraInfo);

	m_ViewMatrixAddress = CameraInfoAddress + Offsets::CCameraInfo::Matrix;

	return false;
}

void Camera::QuickUpdateViewMatrix(DMA_Connection* Conn)
{
	if (!m_ViewMatrixAddress) [[unlikely]] return;

	Matrix44 NewMatrix = EFT::GetProcess().ReadMem<Matrix44>(Conn, m_ViewMatrixAddress);

	std::scoped_lock lk(m_MatrixMutex);
	m_ViewMatrix = NewMatrix;
}

bool Camera::InitializeCameraCache(DMA_Connection* Conn)
{
	if (!m_CameraHeadAddress || !m_NumCameras)
		throw std::runtime_error("Camera system not initialized.");

	auto& Proc = EFT::GetProcess();

	struct NameBuff { char Name[64]{ 0 }; };

	m_CameraCache.clear();

	for (int i = 0; i < m_NumCameras; i++)
	{
		uintptr_t CameraAddr = Proc.ReadMem<uintptr_t>(Conn, m_CameraHeadAddress + (i * sizeof(uintptr_t)));
		uintptr_t GameObjectAddress = Proc.ReadMem<uintptr_t>(Conn, CameraAddr + Offsets::CComponent::pGameObject);
		uintptr_t NameAddress = Proc.ReadMem<uintptr_t>(Conn, GameObjectAddress + Offsets::CGameObject::pName);
		NameBuff Buffer = Proc.ReadMem<NameBuff>(Conn, NameAddress);
		std::string NameStr(Buffer.Name);
		m_CameraCache.push_back({ CameraAddr, NameStr });
	}

	std::println("[Camera] Cached {} cameras.", m_CameraCache.size());

	return true;
}

uintptr_t Camera::SearchCameraCacheByName(const std::string& Name)
{
	for (auto& Entry : m_CameraCache)
	{
		if (Entry.Name == Name)
			return Entry.Address;
	}

	return 0;
}

Matrix44 Camera::GetViewMatrix()
{
	std::scoped_lock lk(m_MatrixMutex);
	return m_ViewMatrix;
}