#pragma once
#include "Game/Classes/Players/CObservedPlayer/CObservedPlayer.h"
#include "Game/Classes/Players/CClientPlayer/CClientPlayer.h"
#include "Game/Classes/CPlayerSkeleton/CPlayerSkeleton.h"
#include "Game/Classes/CHeldItem/CHeldItem.h"

class DrawESPPlayers
{
public:
	static void DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList);

public:
	static inline bool bNameText{ true };
	static inline bool bSkeleton{ true };
	static inline bool bHeadDot{ true };

private:
	static void DrawGenericPlayerText(const CBaseEFTPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList, const ImColor& Color, uint8_t& LineNumber);
	static void DrawObservedPlayerHealthText(const CObservedPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList, uint8_t& LineNumber);
	static void DrawPlayerWeapon(const CHeldItem* pHands, const ImVec2& WindowPos, ImDrawList* DrawList, uint8_t& LineNumber);
	static void Draw(const CObservedPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList);
	static void Draw(const CClientPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList);
	static void DrawSkeleton(const CPlayerSkeleton& Skeleton, const ImVec2& WindowPos, ImDrawList* DrawList);

private:
	static inline Vector3 m_LatestLocalPlayerPos{};
	static inline std::array<Vector2, SKELETON_NUMBONES> m_ProjectedBoneCache{};
};