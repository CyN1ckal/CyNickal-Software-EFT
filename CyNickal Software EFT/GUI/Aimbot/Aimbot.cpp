#include "pch.h"
#include "Aimbot.h"
#include "DMA/Input Manager.h"
#include "Game/Player List/Player List.h"
#include "Game/Camera/Camera.h"
#include "GUI/Fuser/Fuser.h"
#include "GUI/Keybinds/Keybinds.h"	

void Aimbot::RenderSettings()
{
	if (!bSettings) return;

	ImGui::Begin("Aimbot Settings", &bSettings);

	if (m_Device.isConnected() == false)
	{
		if (ImGui::Button("Connect Makcu Device")) m_Device.connect();
	}
	else
		ImGui::Text("Makcu Device Connected: %s", m_Device.getDeviceInfo().port.c_str());

	ImGui::Checkbox("Master Toggle", &bMasterToggle);
	ImGui::Checkbox("Draw FOV Circle", &bDrawFOV);
	ImGui::SliderFloat("Dampen", &fDampen, 0.01f, 1.0f);
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
void Aimbot::OnDMAFrame(DMA_Connection* Conn)
{
	if (!bMasterToggle) return;

	if (c_keys::IsInitialized() == false || m_Device.isConnected() == false) return;

	if (c_keys::IsKeyDown(Conn, m_Keybind) == false) return;

	auto BestTarget = Aimbot::FindBestTarget();

	PlayerList::QuickUpdate(Conn);
	Camera::QuickUpdateViewMatrix(Conn);

	auto Delta = GetAimDeltaToTarget(BestTarget);
	static ImVec2 PreviousDelta{};

	if (Distance(Delta, PreviousDelta) < 0.5f)
		return;

	PreviousDelta = Delta;
	Delta.x *= fDampen;
	Delta.y *= fDampen;

	m_Device.mouseMove(Delta.x, Delta.y);

}

ImVec2 Aimbot::GetAimDeltaToTarget(uintptr_t TargetAddress)
{
	ImVec2 Return{};

	if (TargetAddress == 0x0) return Return;

	auto CenterScreen = Fuser::GetCenterScreen();

	auto TargetWorldPos = PlayerList::GetPlayerBonePosition(TargetAddress, EBoneIndex::Head);

	Vector2 ScreenPos{};
	if (!Camera::WorldToScreen(TargetWorldPos, ScreenPos)) return Return;

	float DistanceFromCenter = Distance(ScreenPos, CenterScreen);

	if (DistanceFromCenter < fDeadzoneFov) return Return;

	if (DistanceFromCenter > fPixelFOV) return Return;

	Return = Subtract(ScreenPos, CenterScreen);

	return Return;
}

uintptr_t Aimbot::FindBestTarget()
{
	std::scoped_lock lk(PlayerList::m_PlayerMutex);

	auto Center = Fuser::GetCenterScreen();
	uintptr_t BestTarget = 0;
	float BestDistance = std::numeric_limits<float>::max();

	for (auto& Player : PlayerList::m_Players)
	{
		std::visit([&](auto& Player) {

			Vector2 ScreenPos{};
			if (!Camera::WorldToScreen(Player.GetBonePosition(EBoneIndex::Head), ScreenPos)) return;

			float DistanceFromCenter = sqrt(pow(ScreenPos.x - Center.x, 2) + pow(ScreenPos.y - Center.y, 2));

			if (DistanceFromCenter > fPixelFOV) return;

			if (DistanceFromCenter < BestDistance)
			{
				BestTarget = Player.m_EntityAddress;
				BestDistance = DistanceFromCenter;
			}

			}, Player);
	}

	return BestTarget;
}