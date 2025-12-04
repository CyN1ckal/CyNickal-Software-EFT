#include "pch.h"

#include "Radar.h"

#include "Game/Player List/Player List.h"

ImVec2 Subtract(const ImVec2& A, const ImVec2& B)
{
	return ImVec2(A.x - B.x, A.y - B.y);
}

void DrawPlayers(ImDrawList* DrawList, ImVec2 WindowPos, ImVec2 WindowSize)
{
	std::scoped_lock Lock(PlayerList::m_PlayerMutex);

	if (PlayerList::m_Players.size() == 0)
		return;

	auto CenterPos = ImVec2(WindowPos.x + WindowSize.x / 2, WindowPos.y + WindowSize.y / 2);
	DrawList->AddCircleFilled(CenterPos, 5, IM_COL32(0, 255, 0, 255));

	auto LocalPlayer = std::get<CClientPlayer>(PlayerList::m_Players[0]);
	auto& LocalPos = LocalPlayer.m_RootPosition;
	//auto& LocalYaw = LocalPlayer.m_Yaw;

	//constexpr float ViewRayLength = 50.0f;
	//constexpr float AnglesToRadians = 0.01745329f;
	//float LocalYawInRadians = LocalYaw * AnglesToRadians;
	//auto ViewRayEndPos = ImVec2(
	//	CenterPos.x + (std::cos(LocalYawInRadians) * ViewRayLength),
	//	CenterPos.y + (std::sin(LocalYawInRadians) * ViewRayLength)
	//);
	//DrawList->AddLine(CenterPos, ViewRayEndPos, IM_COL32(0, 0, 255, 255), 2.0f);

	for (int i = 1; i < PlayerList::m_Players.size(); i++)
	{
		auto& Player = PlayerList::m_Players[i];

		std::visit([LocalPos, CenterPos, DrawList](auto& Player) {
			if (Player.IsInvalid())
				return;

			auto& PlayerPos = Player.m_RootPosition;

			auto Delta3D = PlayerPos - LocalPos;

			Delta3D.x *= Radar::fScale;
			Delta3D.z *= Radar::fScale;

			ImVec2 DotPosition = ImVec2(CenterPos.x + Delta3D.z, CenterPos.y + Delta3D.x);
			DrawList->AddCircleFilled(DotPosition, 5, IM_COL32(255, 0, 0, 255));
			}, Player);
	}
}

void Radar::Render()
{
	ImGui::Begin("Radar");

	auto WindowPos = ImGui::GetWindowPos();
	auto WindowSize = ImGui::GetWindowSize();
	auto DrawList = ImGui::GetWindowDrawList();

	ImVec2 RectTopLeft = WindowPos;
	ImVec2 RectBottomRight = ImVec2(WindowPos.x + WindowSize.x, WindowPos.y + WindowSize.y);

	DrawList->AddRectFilled(RectTopLeft, RectBottomRight, IM_COL32(55, 55, 55, 255));

	DrawPlayers(DrawList, WindowPos, WindowSize);

	ImGui::SliderFloat("Scale", &Radar::fScale, 0.5f, 10.0f, "%.1f");

	ImGui::End();
}