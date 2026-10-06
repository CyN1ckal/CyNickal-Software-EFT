/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Aimbot.h"
#include "DMA/CInputManager/CInputManager.h"
#include "Game/Camera List/Camera List.h"
#include "GUI/Windows/Fuser/Fuser.h"
#include "GUI/Windows/Keybinds/Keybinds.h"	
#include "Game/EFT.h"
#include "Makcu/MyMakcu.h"

void Aimbot::RenderSettings()
{
	if (!bSettings) return;

	ImGui::Begin("Aimbot Settings", &bSettings);
	ImGui::Checkbox("Master Toggle", &bMasterToggle);
	ImGui::Checkbox("Draw FOV Circle", &bDrawFOV);
	ImGui::SliderFloat("Smooth X", &fSmoothX, 1.0f, 50.0f, "%.1f");
	ImGui::SliderFloat("Smooth Y", &fSmoothY, 1.0f, 50.0f, "%.1f");
	ImGui::SliderFloat("FOV", &fPixelFOV, 1.0f, 300.0f);
	ImGui::SliderFloat("Deadzone FOV", &fDeadzoneFov, 1.0f, 10.0f);

	ImGui::End();
}

void Aimbot::RenderFOVCircle(const ImVec2& WindowPos, ImDrawList* DrawList)
{
	if (!bMasterToggle || !bDrawFOV) return;

	auto WindowSize = ImGui::GetWindowSize();
	auto Center = ImVec2(WindowPos.x + WindowSize.x / 2.0f, WindowPos.y + WindowSize.y / 2.0f);
	DrawList->AddCircle(Center, fPixelFOV, IM_COL32(255, 255, 255, 255), 100, 2.0f);
	DrawList->AddCircle(Center, fDeadzoneFov, IM_COL32(255, 0, 0, 255), 100, 2.0f);
}

ImVec2 Subtract(const ImVec2& lhs, const ImVec2& rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}
ImVec2 Subtract(const Vector2& lhs, const ImVec2& rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}
float Distance(Vector2 a, ImVec2 b)
{
	return sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2));
}
float Distance(ImVec2 a, ImVec2 b)
{
	return sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2));
}

void Aimbot::OnDMAFrame(CDMAConnection* Conn)
{
	if (!bMasterToggle) return;
	if (CInputManager::IsInitialized() == false || MyMakcu::m_Device.isConnected() == false) return;
	if (Keybinds::Aimbot.IsActive(Conn) == false) return;

	auto BestTarget = Aimbot::FindBestTarget();
	auto& RegisteredPlayers = EFT::GetRegisteredPlayers();

	do
	{
		auto frameStart = std::chrono::high_resolution_clock::now();

		RegisteredPlayers.QuickUpdate(Conn);
		CameraList::QuickUpdateNecessaryCameras(Conn);

		auto Delta = GetAimDeltaToTarget(BestTarget);
		static ImVec2 PreviousDelta{};
		float fDistance = Distance(Delta, PreviousDelta);

		if (fDistance < 2.0f)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			continue;
		}

		PreviousDelta = Delta;

		float smoothX = std::max(1.0f, fSmoothX);
		float smoothY = std::max(1.0f, fSmoothY);

		Vector2 MoveAmount{ Delta.x / smoothX, Delta.y / smoothY };

		MyMakcu::m_Device.mouseMove(MoveAmount.x, MoveAmount.y);

		// Limit to ~60 FPS to prevent jitter
		auto frameEnd = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);
		if (elapsed.count() < 16) // ~60 FPS
			std::this_thread::sleep_for(std::chrono::milliseconds(16 - elapsed.count()));

	} while (Keybinds::Aimbot.IsActive(Conn));
}

ImVec2 Aimbot::GetAimDeltaToTarget(uintptr_t TargetAddress)
{
	ImVec2 Return{};

	if (TargetAddress == 0x0) return Return;

	auto CenterScreen = Fuser::GetCenterScreen();

	auto TargetWorldPos = EFT::GetRegisteredPlayers().GetPlayerBonePosition(TargetAddress, EBoneIndex::Head);

	Vector2 ScreenPos{};
	if (!CameraList::W2S(TargetWorldPos, ScreenPos)) return Return;

	float DistanceFromCenter = Distance(ScreenPos, CenterScreen);

	if (DistanceFromCenter < fDeadzoneFov) return Return;

	if (DistanceFromCenter > fPixelFOV) return Return;

	Return = Subtract(ScreenPos, CenterScreen);

	return Return;
}

uintptr_t Aimbot::FindBestTarget()
{
	auto& PlayerList = EFT::GetRegisteredPlayers();

	auto Center = Fuser::GetCenterScreen();
	uintptr_t BestTarget = 0;
	float BestDistance = std::numeric_limits<float>::max();

	const auto FindTarget = [&](const CRegisteredPlayers::Player& Player) -> void {
		std::visit([&](auto& Player) {
			Vector2 ScreenPos{};
			if (!CameraList::W2S(Player.GetBonePosition(EBoneIndex::Head), ScreenPos)) return;

			float DistanceFromCenter = sqrt(pow(ScreenPos.x - Center.x, 2) + pow(ScreenPos.y - Center.y, 2));

			if (DistanceFromCenter > fPixelFOV) return;

			if (DistanceFromCenter < BestDistance)
			{
				BestTarget = Player.m_EntityAddress;
				BestDistance = DistanceFromCenter;
			}

			}, Player);
	};

	PlayerList.ForEach_lk(FindTarget);

	return BestTarget;
}