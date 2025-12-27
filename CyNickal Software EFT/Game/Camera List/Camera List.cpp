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

bool CameraList::WorldToScreenEx(const Vector3 WorldPosition, Vector2& ScreenPosition, CCamera* FPSCamera, CCamera* OpticCamera)
{
	auto Transposed = (OpticCamera) ? TransposeMatrix(OpticCamera->GetViewMatrix()) : TransposeMatrix(FPSCamera->GetViewMatrix());
	Vector3 TranslationVector{ Transposed.M[3][0],Transposed.M[3][1],Transposed.M[3][2] };
	Vector3 Up{ Transposed.M[1][0],Transposed.M[1][1],Transposed.M[1][2] };
	Vector3 Right{ Transposed.M[0][0],Transposed.M[0][1],Transposed.M[0][2] };

	float w = DotProduct(TranslationVector, WorldPosition) + Transposed.M[3][3];

	if (w < 0.098f)
		return false;

	float x = DotProduct(WorldPosition, Right) + Transposed.M[0][3];
	float y = DotProduct(WorldPosition, Up) + Transposed.M[1][3];

	if (OpticCamera)
	{
		float angleRadHalf = (std::numbers::pi / 180.f) * FPSCamera->GetFOV() * 0.5f;
		float angleCtg = std::cos(angleRadHalf) / std::sin(angleRadHalf);
		x /= angleCtg * FPSCamera->GetAspectRatio() * 0.5f;
		y /= angleCtg * 0.5f;
	}

	auto CenterScreen = Fuser::GetCenterScreen();
	ScreenPosition.x = (CenterScreen.x) * (1.f + x / w);
	ScreenPosition.y = (CenterScreen.y) * (1.f - y / w);

	return true;
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

	return false;
}

bool CameraList::W2S(const Vector3 WorldPosition, Vector2& ScreenPosition)
{
	if (m_pFPSCamera == nullptr)
		return false;

	return WorldToScreenEx(WorldPosition, ScreenPosition, m_pFPSCamera);
}

bool CameraList::OpticW2S(const Vector3 WorldPosition, Vector2& ScreenPosition)
{
	auto OpticCamera = GetSelectedOptic();
	if (OpticCamera == nullptr || m_pFPSCamera == nullptr)
		return false;

	return WorldToScreenEx(WorldPosition, ScreenPosition, m_pFPSCamera, OpticCamera);
}

CCamera* CameraList::GetSelectedOptic()
{
	if (m_OpticIndex < m_pOpticCameras.size())
		return m_pOpticCameras[m_OpticIndex];

	return nullptr;
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

	m_pOpticCameras = GetPotentialOpticCameras();

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

std::vector<CCamera*> CameraList::GetPotentialOpticCameras()
{
	std::vector<CCamera*> PotentialOptics;

	for (auto& Entry : m_CameraCache)
	{
		if (Entry.GetName().find("OpticCamera") != std::string::npos)
		{
			std::println("[Camera] Found potential optic camera: {}", Entry.GetName());
			PotentialOptics.push_back(&Entry);
		}
	}

	return PotentialOptics;
}

CCamera* CameraList::FindWinningOptic(const std::vector<CCamera*>& PotentialOpticCams)
{
	for (auto& PotentialOpticCam : PotentialOpticCams)
	{
		if (PotentialOpticCam->IsInvalid())
			continue;

		auto ViewMatrix = PotentialOpticCam->GetViewMatrix();

		if (ViewMatrix.M[3][0] < 0.001f && ViewMatrix.M[3][1] < 0.001f && ViewMatrix.M[3][2] < 0.001f)
		{
			std::println("[Camera] Skipping optic camera {} due to zero translation vector.", PotentialOpticCam->GetName());
			continue;
		}

		if (ViewMatrix.M[3][3] < 0.1f)
		{
			std::println("[Camera] Skipping optic camera {} due to invalid view matrix.", PotentialOpticCam->GetName());
			continue;
		}

		float RightMagnitude = std::sqrt(ViewMatrix.M[0][0] * ViewMatrix.M[0][0] + ViewMatrix.M[0][1] * ViewMatrix.M[0][1] + ViewMatrix.M[0][2] * ViewMatrix.M[0][2]);
		float UpMagnitude = std::sqrt(ViewMatrix.M[1][0] * ViewMatrix.M[1][0] + ViewMatrix.M[1][1] * ViewMatrix.M[1][1] + ViewMatrix.M[1][2] * ViewMatrix.M[1][2]);
		float ForwardMagnitude = std::sqrt(ViewMatrix.M[2][0] * ViewMatrix.M[2][0] + ViewMatrix.M[2][1] * ViewMatrix.M[2][1] + ViewMatrix.M[2][2] * ViewMatrix.M[2][2]);
		if (RightMagnitude < 0.1f && UpMagnitude < 0.1f && ForwardMagnitude < 0.1f)
		{
			std::println("[Camera] Skipping optic camera {} due to invalid vector magnitude.", PotentialOpticCam->GetName());
			continue;
		}

		std::println("[Camera] Found winning optic camera: {}", PotentialOpticCam->GetName());

		return PotentialOpticCam;
	}

	std::println("[Camera] Failed to find a valid optic camera.");

	return nullptr;
}

void CameraList::QuickUpdateNecessaryCameras(DMA_Connection* Conn)
{
	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), EFT::GetProcess().GetPID(), VMMDLL_FLAG_NOCACHE);

	auto pSelectedOptic = GetSelectedOptic();

	if (m_pFPSCamera)
		m_pFPSCamera->QuickRead(vmsh);

	if (pSelectedOptic)
		pSelectedOptic->QuickRead(vmsh);

	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	if (m_pFPSCamera)
		m_pFPSCamera->QuickFinalize();

	if (pSelectedOptic)
		pSelectedOptic->QuickFinalize();
}

/*
	This is temporary and should be replaced with the optic's true width and center
*/
static float fOpticRadius{ 300.0f };
float CameraList::GetOpticRadius()
{
	return fOpticRadius;
}
void CameraList::SetOpticRadius(float Width)
{
	fOpticRadius = Width;
}
Vector2 CameraList::GetOpticCenter()
{
	auto WindowSize = ImGui::GetWindowSize();

	return Vector2(WindowSize.x * 0.5f, WindowSize.y * 0.5f);
}