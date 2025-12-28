#include "pch.h"
#include "Players.h"
#include "Game/Camera List/Camera List.h"
#include "Game/Enums/EBoneIndex.h"
#include "GUI/Color Picker/Color Picker.h"
#include "Game/EFT.h"

void DrawESPPlayers::DrawObservedPlayer(const CObservedPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList, std::array<ProjectedBoneInfo, SKELETON_NUMBONES>& ProjectedBones, bool bForOptic)
{
	if (Player.IsInvalid())	return;

	if (Player.m_pSkeleton == nullptr) return;

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
		DrawPlayerWeapon(Player.m_pHands.get(), WindowPos, DrawList, LineNumber, ProjectedBones);
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

	if (Player.IsLocalPlayer())	return;

	if (Player.m_pSkeleton == nullptr) return;

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
		DrawPlayerWeapon(Player.m_pHands.get(), WindowPos, DrawList, LineNumber, ProjectedBones);
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
	auto& PlayerList = EFT::GetRegisteredPlayers();

	m_LatestLocalPlayerPos = PlayerList.GetLocalPlayerPosition();

	std::scoped_lock lk(PlayerList.m_Mut);

	auto LocalPlayer = PlayerList.GetLocalPlayer();
	if (LocalPlayer == nullptr || LocalPlayer->IsInvalid()) return;

	auto bDrawOpticESP = LocalPlayer->IsAiming() && bOpticESP;
	auto WindowSize = ImGui::GetWindowSize();
	auto OpticFOV = CameraList::GetOpticRadius();

	if (PlayerList.m_Players.empty()) return;

	for (auto& Player : PlayerList.m_Players)
		std::visit([WindowPos, DrawList](auto& Player) { DrawESPPlayers::Draw(Player, WindowPos, DrawList); }, Player);

	if (bDrawOpticESP)
	{
		ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(WindowPos.x + (WindowSize.x * 0.5f), WindowPos.y + (WindowSize.y * 0.5f)), OpticFOV, ImColor(0, 0, 0), 33);
		ImGui::GetWindowDrawList()->AddCircle(ImVec2(WindowPos.x + (WindowSize.x * 0.5f), WindowPos.y + (WindowSize.y * 0.5f)), OpticFOV + 2.0f, ImColor(255, 255, 255), 33, 2.f);

		for (auto& Player : PlayerList.m_Players)
			std::visit([WindowPos, DrawList](auto& Player) { DrawESPPlayers::DrawForOptic(Player, WindowPos, DrawList); }, Player);
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
	std::string Text = std::format("{0:s} [{1:.0f}m]", Player.GetBaseName(), Player.GetBonePosition(EBoneIndex::Root).DistanceTo(m_LatestLocalPlayerPos));
	auto& ProjectedRootPos = ProjectedBones[Sketon_MyIndicies[EBoneIndex::Root]];
	DrawTextAtPosition(DrawList, ImVec2(WindowPos.x + ProjectedRootPos.ScreenPos.x, WindowPos.y + ProjectedRootPos.ScreenPos.y + (ImGui::GetTextLineHeight() * LineNumber)), Player.GetFuserColor(), Text);
	LineNumber++;
}

const std::string InjuredString = "(Injured)";
const std::string BadlyInjuredString = "(Badly Injured)";
const std::string DyingString = "(Dying)";

void DrawESPPlayers::DrawObservedPlayerHealthText(const CObservedPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList, uint8_t& LineNumber, std::array<ProjectedBoneInfo, SKELETON_NUMBONES>& ProjectedBones)
{
	const char* DataPtr = nullptr;
	if (Player.IsInCondition(ETagStatus::Injured))
		DataPtr = InjuredString.data();
	else if (Player.IsInCondition(ETagStatus::BadlyInjured))
		DataPtr = BadlyInjuredString.data();
	else if (Player.IsInCondition(ETagStatus::Dying))
		DataPtr = DyingString.data();

	if (DataPtr == nullptr) return;

	auto& ProjectedRootPos = ProjectedBones[Sketon_MyIndicies[EBoneIndex::Root]];
	DrawTextAtPosition(DrawList, ImVec2(WindowPos.x + ProjectedRootPos.ScreenPos.x, WindowPos.y + ProjectedRootPos.ScreenPos.y + (ImGui::GetTextLineHeight() * LineNumber)), Player.GetFuserColor(), DataPtr);
	LineNumber++;
}

void DrawESPPlayers::DrawPlayerWeapon(const CHeldItem* pHands, const ImVec2& WindowPos, ImDrawList* DrawList, uint8_t& LineNumber, const std::array<ProjectedBoneInfo, SKELETON_NUMBONES>& ProjectedBones)
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
	if (Magazine == nullptr) return;

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