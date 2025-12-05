#pragma once
#include "Game/Classes/Players/CBaseEFTPlayer/CBaseEFTPlayer.h"

class CClientPlayer : public CBaseEFTPlayer
{
public:
	uintptr_t m_MovementContextAddress{ 0 };
	uintptr_t m_ProfileAddress{ 0 };
	uintptr_t m_ProfileInfoAddress{ 0 };

public:
	CClientPlayer(uintptr_t EntityAddress) : CBaseEFTPlayer(EntityAddress) {}
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
};