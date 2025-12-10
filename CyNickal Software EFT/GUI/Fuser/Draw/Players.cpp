#include "pch.h"
#include "Players.h"
#include "Game/Player List/Player List.h"
#include "Game/Camera/Camera.h"
#include "Game/Enums/EBoneIndex.h"

void DrawESPPlayers::DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList)
{
	m_LatestLocalPlayerPos = PlayerList::GetLocalPlayerPosition();

	std::scoped_lock lk(PlayerList::m_PlayerMutex);

	if (PlayerList::m_Players.empty()) return;

	for (auto& Player : PlayerList::m_Players)
		std::visit([WindowPos, DrawList](auto& Player) { DrawESPPlayers::Draw(Player, WindowPos, DrawList); }, Player);
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

void  DrawESPPlayers::DrawGenericPlayerText(const CBaseEFTPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList, const ImColor& Color, uint8_t& LineNumber)
{
	std::string Text = std::format("{0:s} [{1:.0f}m]", Player.GetBaseName(), Player.GetBonePosition(EBoneIndex::Root).DistanceTo(m_LatestLocalPlayerPos));
	auto& ProjectedRootPos = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::Root]];
	DrawTextAtPosition(DrawList, ImVec2(WindowPos.x + ProjectedRootPos.x, WindowPos.y + ProjectedRootPos.y + (ImGui::GetTextLineHeight() * LineNumber)), Player.GetSideColor(), Text);
	LineNumber++;
}

const std::string InjuredString = "(Injured)";
const std::string BadlyInjuredString = "(Badly Injured)";
const std::string DyingString = "(Dying)";

void DrawESPPlayers::DrawObservedPlayerHealthText(const CObservedPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList, uint8_t& LineNumber)
{
	const char* DataPtr = nullptr;
	if (Player.IsInCondition(ETagStatus::Injured))
		DataPtr = InjuredString.data();
	else if (Player.IsInCondition(ETagStatus::BadlyInjured))
		DataPtr = BadlyInjuredString.data();
	else if (Player.IsInCondition(ETagStatus::Dying))
		DataPtr = DyingString.data();

	if (DataPtr == nullptr) return;

	auto& ProjectedRootPos = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::Root]];
	DrawTextAtPosition(DrawList, ImVec2(WindowPos.x + ProjectedRootPos.x, WindowPos.y + ProjectedRootPos.y + (ImGui::GetTextLineHeight() * LineNumber)), Player.GetSideColor(), DataPtr);
	LineNumber++;
}

void DrawESPPlayers::DrawPlayerWeapon(const CHeldItem* pHands, const ImVec2& WindowPos, ImDrawList* DrawList, uint8_t& LineNumber)
{
	if (pHands == nullptr)
		return;

	if (pHands->IsInvalid()) return;

	auto TextSize = ImGui::CalcTextSize(pHands->m_HeldItem.GetItemName());
	auto& ProjectedRootPos = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::Root]];
	DrawList->AddText(
		ImVec2(WindowPos.x + ProjectedRootPos.x - (TextSize.x / 2.0f), WindowPos.y + ProjectedRootPos.y + (ImGui::GetTextLineHeight() * LineNumber)),
		ImColor(255, 255, 255, 255),
		pHands->m_HeldItem.GetItemName()
	);
	LineNumber++;
}

void DrawESPPlayers::Draw(const CObservedPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList)
{
	if (Player.IsInvalid())	return;

	if (Player.m_pSkeleton == nullptr) return;

	m_ProjectedBoneCache.fill({});

	for (int i = 0; i < SKELETON_NUMBONES; i++)
		Camera::WorldToScreen(Player.m_pSkeleton->m_BonePositions[i], m_ProjectedBoneCache[i]);

	uint8_t LineNumber = 0;

	if (bNameText) {
		DrawGenericPlayerText(Player, WindowPos, DrawList, Player.GetSideColor(), LineNumber);
		DrawPlayerWeapon(Player.m_pHands.get(), WindowPos, DrawList, LineNumber);
		DrawObservedPlayerHealthText(Player, WindowPos, DrawList, LineNumber);
	}

	if (bHeadDot) {
		auto& ProjectedHeadPos = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::Head]];
		DrawList->AddCircle(ImVec2(WindowPos.x + ProjectedHeadPos.x, WindowPos.y + ProjectedHeadPos.y), 4.0f, Player.GetSideColor(), 12, 1.0f);
	}

	if (bSkeleton)
		DrawSkeleton(*Player.m_pSkeleton, WindowPos, DrawList);
}

void DrawESPPlayers::Draw(const CClientPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList)
{
	if (Player.IsInvalid())	return;

	if (Player.IsLocalPlayer())	return;

	if (Player.m_pSkeleton == nullptr) return;

	m_ProjectedBoneCache.fill({});

	for (int i = 0; i < SKELETON_NUMBONES; i++)
		Camera::WorldToScreen(Player.m_pSkeleton->m_BonePositions[i], m_ProjectedBoneCache[i]);

	uint8_t LineNumber = 0;

	if (bNameText) {
		DrawGenericPlayerText(Player, WindowPos, DrawList, Player.GetSideColor(), LineNumber);
		DrawPlayerWeapon(Player.m_pHands.get(), WindowPos, DrawList, LineNumber);
	}

	if (bHeadDot) {
		auto& ProjectedHeadPos = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::Head]];
		DrawList->AddCircle(ImVec2(WindowPos.x + ProjectedHeadPos.x, WindowPos.y + ProjectedHeadPos.y), 4.0f, Player.GetSideColor(), 12, 1.0f);
	}

	if (bSkeleton)
		DrawSkeleton(*Player.m_pSkeleton, WindowPos, DrawList);
}

void ConnectBones(const Vector2& BoneA, const Vector2& BoneB, const ImVec2& WindowPos, ImDrawList* DrawList, const ImColor& Color, float Thickness)
{
	DrawList->AddLine(
		{ WindowPos.x + BoneA.x, WindowPos.y + BoneA.y },
		{ WindowPos.x + BoneB.x, WindowPos.y + BoneB.y },
		Color,
		Thickness
	);
}

void DrawESPPlayers::DrawSkeleton(const CPlayerSkeleton& Skeleton, const ImVec2& WindowPos, ImDrawList* DrawList)
{
	auto& ProjectedHead = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::Head]];
	auto& ProjectedNeck = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::Neck]];
	auto& ProjectedSpine = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::Spine3]];
	auto& ProjectedPelvis = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::Pelvis]];
	auto& ProjectedLThigh1 = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::LThigh1]];
	auto& ProjectedLThigh2 = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::LThigh2]];
	auto& ProjectedLCalf = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::LCalf]];
	auto& ProjectedLFoot = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::LFoot]];
	auto& ProjectedRThigh1 = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::RThigh1]];
	auto& ProjectedRThigh2 = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::RThigh2]];
	auto& ProjectedRCalf = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::RCalf]];
	auto& ProjectedRFoot = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::RFoot]];
	auto& ProjectedRUpperArm = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::RUpperArm]];
	auto& ProjectedRForeArm1 = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::RForeArm1]];
	auto& ProjectedRForeArm2 = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::RForeArm2]];
	auto& ProjectedRPalm = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::RPalm]];
	auto& ProjectedLUpperArm = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::LUpperArm]];
	auto& ProjectedLForeArm1 = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::LForeArm1]];
	auto& ProjectedLForeArm2 = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::LForeArm2]];
	auto& ProjectedLPalm = m_ProjectedBoneCache[Sketon_MyIndicies[EBoneIndex::LPalm]];

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