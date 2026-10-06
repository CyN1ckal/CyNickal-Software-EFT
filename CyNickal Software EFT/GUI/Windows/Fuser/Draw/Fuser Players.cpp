/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Fuser Players.h"
#include "Game/Camera List/Camera List.h"
#include "Game/Enums/EBoneIndex.h"
#include "GUI/Windows/Color Picker/Color Picker.h"
#include "Game/EFT.h"
#include "GUI/MyImGui/MyImGui.h"

void DrawESPPlayers::DrawObservedPlayer(const CObservedPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList, std::array<ProjectedBoneInfo, SKELETON_NUMBONES>& ProjectedBones, bool bForOptic)
{
	if (Player.IsInvalid())	return;

	if (Player.IsDead()) return;

	if (Player.m_pSkeleton == std::nullopt) return;

	ProjectedBones.fill({});

	for (int i = 0; i < SKELETON_NUMBONES; i++)
		ProjectedBones[i].bIsOnScreen = bForOptic ? CameraList::OpticW2S(Player.m_pSkeleton->m_BonePositions[i], ProjectedBones[i].ScreenPos) : CameraList::W2S(Player.m_pSkeleton->m_BonePositions[i], ProjectedBones[i].ScreenPos);

	if (bForOptic
		&& ProjectedBones[Sketon_MyIndicies[EBoneIndex::Root]].ScreenPos.DistanceTo(CameraList::GetOpticCenter()) > CameraList::GetOpticRadius()
		&& ProjectedBones[Sketon_MyIndicies[EBoneIndex::Head]].ScreenPos.DistanceTo(CameraList::GetOpticCenter()) > CameraList::GetOpticRadius())
		return;

	uint8_t LineNumber = 0;

	if (bNameText) {
		DrawGenericPlayerText(Player, WindowPos, DrawList, Player.GetFuserColor(), LineNumber, ProjectedBones);
		DrawPlayerWeapon(&Player.m_pHands.value(), WindowPos, DrawList, LineNumber, ProjectedBones);
		DrawObservedPlayerHealthText(Player, WindowPos, DrawList, LineNumber, ProjectedBones);
	}

	if (bHeadDot) {
		auto& ProjectedHeadPos = ProjectedBones[Sketon_MyIndicies[EBoneIndex::Head]];
		DrawList->AddCircle(ImVec2(WindowPos.x + ProjectedHeadPos.ScreenPos.x, WindowPos.y + ProjectedHeadPos.ScreenPos.y), 4.0f, Player.GetFuserColor(), 12, 1.0f);
	}

	if (bSkeleton)
		DrawSkeleton(WindowPos, DrawList, ProjectedBones);
}

void DrawESPPlayers::DrawClientPlayer(const CClientPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList, std::array<ProjectedBoneInfo, SKELETON_NUMBONES>& ProjectedBones, bool bForOptic)
{
	if (Player.IsInvalid())	return;

	if (Player.IsDead()) return;

	if (Player.IsLocalPlayer()) {
		if (Player.m_pHands && Player.m_pHands->m_FireportTransform && Player.m_pHands->m_FireportTransform->IsInvalid() == false) {
			/*auto FireportWorldPos = Player.m_pHands->m_FireportPosition;
			Vector2 FireportScreenPos{};
			if (!CameraList::W2S(FireportWorldPos, FireportScreenPos)) return;

			ImVec2 FireportDrawPos = { WindowPos.x + FireportScreenPos.x, WindowPos.y + FireportScreenPos.y };
			DrawList->AddCircle(FireportDrawPos, 10.0f, ImColor(255, 0, 0, 255), 12, 2.0f);*/
			/*auto Quat = Player.m_pHands->m_FireportTransform->GetRotation();
			std::string FireportRotationText = std::format("Fireport Rotation: {0:.2f}, {1:.2f}, {2:.2f}, {3:.2f}", Quat.x, Quat.y, Quat.z, Quat.w);
			DrawList->AddText(ImVec2(FireportDrawPos.x - (ImGui::CalcTextSize(FireportRotationText.c_str()).x / 2.0f), FireportDrawPos.y + 12.0f), ImColor(255, 255, 255, 255), FireportRotationText.c_str());*/
		}
		return;
	}

	if (Player.m_pSkeleton == std::nullopt) return;

	ProjectedBones.fill({});

	for (int i = 0; i < SKELETON_NUMBONES; i++)
		ProjectedBones[i].bIsOnScreen = (bForOptic) ? CameraList::OpticW2S(Player.m_pSkeleton->m_BonePositions[i], ProjectedBones[i].ScreenPos) : CameraList::W2S(Player.m_pSkeleton->m_BonePositions[i], ProjectedBones[i].ScreenPos);

	if (bForOptic
		&& ProjectedBones[Sketon_MyIndicies[EBoneIndex::Root]].ScreenPos.DistanceTo(CameraList::GetOpticCenter()) > CameraList::GetOpticRadius()
		&& ProjectedBones[Sketon_MyIndicies[EBoneIndex::Head]].ScreenPos.DistanceTo(CameraList::GetOpticCenter()) > CameraList::GetOpticRadius())
		return;

	uint8_t LineNumber = 0;

	if (bNameText) {
		DrawGenericPlayerText(Player, WindowPos, DrawList, Player.GetFuserColor(), LineNumber, ProjectedBones);
		//DrawPlayerWeapon(&Player.m_pHands.value(), WindowPos, DrawList, LineNumber, ProjectedBones);
	}

	if (bHeadDot) {
		auto& ProjectedHeadPos = ProjectedBones[Sketon_MyIndicies[EBoneIndex::Head]];
		DrawList->AddCircle(ImVec2(WindowPos.x + ProjectedHeadPos.ScreenPos.x, WindowPos.y + ProjectedHeadPos.ScreenPos.y), 4.0f, Player.GetFuserColor(), 12, 1.0f);
	}

	if (bSkeleton)
		DrawSkeleton(WindowPos, DrawList, ProjectedBones);
}

void DrawESPPlayers::DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList)
{
	if (!EFT::pGameWorld->m_pRegisteredPlayers) return;

	ZoneScoped;

	auto& PlayerList = EFT::GetRegisteredPlayers();

	m_LatestLocalPlayerPos = PlayerList.GetLocalPlayerPosition();

	auto bDrawOpticESP = PlayerList.IsLocalPlayerAiming() && bOpticESP;
	auto WindowSize = ImGui::GetWindowSize();
	auto OpticFOV = CameraList::GetOpticRadius();

	const auto DrawPlayer = [&](const CRegisteredPlayers::Player& Player) {
		std::visit([WindowPos, DrawList](auto& Player) { DrawESPPlayers::Draw(Player, WindowPos, DrawList); }, Player);
		};

	PlayerList.ForEach_lk(DrawPlayer);

	if (bDrawOpticESP)
	{
		ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(WindowPos.x + (WindowSize.x * 0.5f), WindowPos.y + (WindowSize.y * 0.5f)), OpticFOV, ImColor(0, 0, 0), 33);
		ImGui::GetWindowDrawList()->AddCircle(ImVec2(WindowPos.x + (WindowSize.x * 0.5f), WindowPos.y + (WindowSize.y * 0.5f)), OpticFOV + 2.0f, ImColor(255, 255, 255), 33, 2.f);

		const auto DrawPlayerForOptic = [&](const CRegisteredPlayers::Player& Player) {
			std::visit([WindowPos, DrawList](auto& Player) { DrawESPPlayers::DrawForOptic(Player, WindowPos, DrawList); }, Player);
			};

		PlayerList.ForEach_lk(DrawPlayerForOptic);
	}
}

void DrawTextAtPosition(ImDrawList* DrawList, const ImVec2& Position, const ImColor& Color, const std::string& Text)
{
	auto TextSize = ImGui::CalcTextSize(Text.c_str());
	DrawList->AddText(
		ImVec2(Position.x - (TextSize.x / 2.0f), Position.y),
		Color,
		Text.c_str()
	);
}

void DrawESPPlayers::DrawGenericPlayerText(const CBaseEFTPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList, const ImColor& Color, uint8_t& LineNumber, std::array<ProjectedBoneInfo, SKELETON_NUMBONES>& ProjectedBones)
{
	auto& ProjectedRootPos = ProjectedBones[Sketon_MyIndicies[EBoneIndex::Root]];
	if (ProjectedRootPos.bIsOnScreen == false) return;

	CTwoToneNameplateInfo NameplateInfo{};
	NameplateInfo.Prefix = Player.GetBaseName();
	NameplateInfo.Name = std::format("{:.0f}m", Player.GetBonePosition(EBoneIndex::Root).DistanceTo(m_LatestLocalPlayerPos));
	NameplateInfo.PrefixBackgroundColor = ImColor(150, 150, 150, 128);
	NameplateInfo.NameBackgroundColor = Player.GetFuserColor();
	MyImGui::DrawTwoToneNameplate(NameplateInfo, ProjectedBones[Sketon_MyIndicies[EBoneIndex::Root]].ScreenPos);

	LineNumber++;
}

const std::string InjuredString = "(Injured)";
const std::string BadlyInjuredString = "(Badly Injured)";
const std::string DyingString = "(Dying)";

void DrawESPPlayers::DrawObservedPlayerHealthText(const CObservedPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList, uint8_t& LineNumber, std::array<ProjectedBoneInfo, SKELETON_NUMBONES>& ProjectedBones)
{
	const char* DataPtr = nullptr;
	if (Player.IsInCondition(ETagStatus::Dying))
		DataPtr = DyingString.data();
	else if (Player.IsInCondition(ETagStatus::BadlyInjured))
		DataPtr = BadlyInjuredString.data();
	else if (Player.IsInCondition(ETagStatus::Injured))
		DataPtr = InjuredString.data();

	if (DataPtr == nullptr) return;

	auto& ProjectedRootPos = ProjectedBones[Sketon_MyIndicies[EBoneIndex::Root]];
	DrawTextAtPosition(DrawList, ImVec2(WindowPos.x + ProjectedRootPos.ScreenPos.x, WindowPos.y + ProjectedRootPos.ScreenPos.y + (ImGui::GetTextLineHeight() * LineNumber)), Player.GetFuserColor(), DataPtr);
	LineNumber++;
}

void DrawESPPlayers::DrawPlayerWeapon(const CHandsController* pHands, const ImVec2& WindowPos, ImDrawList* DrawList, uint8_t& LineNumber, const std::array<ProjectedBoneInfo, SKELETON_NUMBONES>& ProjectedBones)
{
	if (!pHands) return;
	if (pHands->IsInvalid()) return;

	auto& HeldItem = pHands->m_pHeldItem;

	auto& ProjectedRootPos = ProjectedBones[Sketon_MyIndicies[EBoneIndex::Root]];
	ImVec2 RootScreenPos = { WindowPos.x + ProjectedRootPos.ScreenPos.x, WindowPos.y + ProjectedRootPos.ScreenPos.y };

	auto ItemName = pHands->m_pHeldItem->GetItemName();

	auto TextSize = ImGui::CalcTextSize(ItemName.c_str());
	DrawList->AddText(
		ImVec2(RootScreenPos.x - (TextSize.x / 2.0f), RootScreenPos.y + (ImGui::GetTextLineHeight() * LineNumber)),
		ColorPicker::Fuser::m_WeaponTextColor,
		ItemName.c_str()
	);
	LineNumber++;

	auto& Magazine = pHands->m_pMagazine;
	if (Magazine == std::nullopt) return;

	std::string MagText = std::format("{0:d} {1:s}", Magazine->m_CurrentCartridges, Magazine->GetAmmoName().c_str());
	TextSize = ImGui::CalcTextSize(MagText.c_str());
	DrawList->AddText(
		ImVec2(RootScreenPos.x - (TextSize.x / 2.0f), RootScreenPos.y + (ImGui::GetTextLineHeight() * LineNumber)),
		ColorPicker::Fuser::m_WeaponTextColor,
		MagText.c_str()
	);
	LineNumber++;
}

void DrawESPPlayers::Draw(const CObservedPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList)
{
	DrawObservedPlayer(Player, WindowPos, DrawList, m_ProjectedBoneCache, false);
}

void DrawESPPlayers::Draw(const CClientPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList)
{
	DrawClientPlayer(Player, WindowPos, DrawList, m_ProjectedBoneCache, false);
}

void DrawESPPlayers::DrawForOptic(const CObservedPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList)
{
	DrawObservedPlayer(Player, WindowPos, DrawList, m_ProjectedBoneCache, true);
}

void DrawESPPlayers::DrawForOptic(const CClientPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList)
{
	DrawClientPlayer(Player, WindowPos, DrawList, m_ProjectedBoneCache, true);
}

void ConnectBones(const ProjectedBoneInfo& BoneA, const ProjectedBoneInfo& BoneB, const ImVec2& WindowPos, ImDrawList* DrawList, const ImColor& Color, float Thickness)
{
	if (BoneA.bIsOnScreen == false || BoneB.bIsOnScreen == false)
		return;

	DrawList->AddLine(
		{ WindowPos.x + BoneA.ScreenPos.x, WindowPos.y + BoneA.ScreenPos.y },
		{ WindowPos.x + BoneB.ScreenPos.x, WindowPos.y + BoneB.ScreenPos.y },
		Color,
		Thickness
	);
}

void DrawESPPlayers::DrawSkeleton(const ImVec2& WindowPos, ImDrawList* DrawList, const std::array<ProjectedBoneInfo, SKELETON_NUMBONES>& ProjectedBones)
{
	auto& ProjectedHead = ProjectedBones[Sketon_MyIndicies[EBoneIndex::Head]];
	auto& ProjectedNeck = ProjectedBones[Sketon_MyIndicies[EBoneIndex::Neck]];
	auto& ProjectedSpine = ProjectedBones[Sketon_MyIndicies[EBoneIndex::Spine3]];
	auto& ProjectedPelvis = ProjectedBones[Sketon_MyIndicies[EBoneIndex::Pelvis]];
	auto& ProjectedLThigh1 = ProjectedBones[Sketon_MyIndicies[EBoneIndex::LThigh1]];
	auto& ProjectedLThigh2 = ProjectedBones[Sketon_MyIndicies[EBoneIndex::LThigh2]];
	auto& ProjectedLCalf = ProjectedBones[Sketon_MyIndicies[EBoneIndex::LCalf]];
	auto& ProjectedLFoot = ProjectedBones[Sketon_MyIndicies[EBoneIndex::LFoot]];
	auto& ProjectedRThigh1 = ProjectedBones[Sketon_MyIndicies[EBoneIndex::RThigh1]];
	auto& ProjectedRThigh2 = ProjectedBones[Sketon_MyIndicies[EBoneIndex::RThigh2]];
	auto& ProjectedRCalf = ProjectedBones[Sketon_MyIndicies[EBoneIndex::RCalf]];
	auto& ProjectedRFoot = ProjectedBones[Sketon_MyIndicies[EBoneIndex::RFoot]];
	auto& ProjectedRUpperArm = ProjectedBones[Sketon_MyIndicies[EBoneIndex::RUpperArm]];
	auto& ProjectedRForeArm1 = ProjectedBones[Sketon_MyIndicies[EBoneIndex::RForeArm1]];
	auto& ProjectedRForeArm2 = ProjectedBones[Sketon_MyIndicies[EBoneIndex::RForeArm2]];
	auto& ProjectedRPalm = ProjectedBones[Sketon_MyIndicies[EBoneIndex::RPalm]];
	auto& ProjectedLUpperArm = ProjectedBones[Sketon_MyIndicies[EBoneIndex::LUpperArm]];
	auto& ProjectedLForeArm1 = ProjectedBones[Sketon_MyIndicies[EBoneIndex::LForeArm1]];
	auto& ProjectedLForeArm2 = ProjectedBones[Sketon_MyIndicies[EBoneIndex::LForeArm2]];
	auto& ProjectedLPalm = ProjectedBones[Sketon_MyIndicies[EBoneIndex::LPalm]];

	constexpr float Width = 2.0f;

	ConnectBones(ProjectedHead, ProjectedNeck, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedNeck, ProjectedSpine, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedSpine, ProjectedPelvis, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedPelvis, ProjectedLThigh1, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedLThigh1, ProjectedLThigh2, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedLThigh2, ProjectedLCalf, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedLCalf, ProjectedLFoot, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedPelvis, ProjectedRThigh1, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedRThigh1, ProjectedRThigh2, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedRThigh2, ProjectedRCalf, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedRCalf, ProjectedRFoot, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedSpine, ProjectedRUpperArm, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedRUpperArm, ProjectedRForeArm1, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedRForeArm1, ProjectedRForeArm2, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedRForeArm2, ProjectedRPalm, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedSpine, ProjectedLUpperArm, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedLUpperArm, ProjectedLForeArm1, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedLForeArm1, ProjectedLForeArm2, WindowPos, DrawList, ImColor(255, 0, 0), Width);
	ConnectBones(ProjectedLForeArm2, ProjectedLPalm, WindowPos, DrawList, ImColor(255, 0, 0), Width);
}