#include "pch.h"
#include "Players.h"
#include "Game/Player List/Player List.h"
#include "Game/Camera/Camera.h"
#include "Game/Enums/EBoneIndex.h"

void DrawESPPlayers::DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList)
{
	std::scoped_lock lk(PlayerList::m_PlayerMutex);

	if (PlayerList::m_Players.empty()) return;

	auto& LocalPlayer = std::get<CClientPlayer>(PlayerList::m_Players[0]);
	if (LocalPlayer.IsInvalid()) return;

	m_LatestLocalPlayerPos = LocalPlayer.GetBonePosition(EBoneIndex::Root);

	for (int i = 1; i < PlayerList::m_Players.size(); i++)
	{
		auto& Player = PlayerList::m_Players[i];
		std::visit([WindowPos, DrawList](auto& Player) { DrawESPPlayers::Draw(Player, WindowPos, DrawList); }, Player);
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

std::array<Vector2, SKELETON_NUMBONES> ProjectedBones{};
void DrawESPPlayers::Draw(const CBaseEFTPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList)
{
	if (Player.IsInvalid())	return;

	if (Player.m_pSkeleton == nullptr) return;

	ProjectedBones.fill({});

	for (int i = 0; i < SKELETON_NUMBONES; i++)
		Camera::WorldToScreen(Player.m_pSkeleton->m_BonePositions[i], ProjectedBones[i]);

	std::string Text = std::format("{0:s} [{1:.0f}m]", Player.GetBaseName(), Player.GetBonePosition(EBoneIndex::Root).DistanceTo(m_LatestLocalPlayerPos), std::to_underlying(Player.m_SpawnType));

	if (bNameText) {
		auto& ProjectedRootPos = ProjectedBones[Sketon_MyIndicies[EBoneIndex::Root]];
		DrawTextAtPosition(DrawList, ImVec2(WindowPos.x + ProjectedRootPos.x, WindowPos.y + ProjectedRootPos.y), Player.GetSideColor(), Text);
	}

	if (bHeadDot) {
		auto& ProjectedHeadPos = ProjectedBones[Sketon_MyIndicies[EBoneIndex::Head]];
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