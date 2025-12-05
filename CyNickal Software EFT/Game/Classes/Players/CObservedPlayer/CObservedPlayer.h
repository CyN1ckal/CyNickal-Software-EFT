#pragma once
#include "Game/Classes/Players/CBaseEFTPlayer/CBaseEFTPlayer.h"

class CObservedPlayer : public CBaseEFTPlayer
{
public:
	uintptr_t m_PlayerControllerAddress{ 0 };
	uintptr_t m_MovementControllerAddress{ 0 };
	uintptr_t m_ObservedMovementStateAddress{ 0 };
	float m_Yaw{ 0.0f };
	std::byte m_IsAiByte{ 0 };

public:
	CObservedPlayer(uintptr_t EntityAddress) : CBaseEFTPlayer(EntityAddress) {}
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);

public:
	const bool IsAi() const;
};