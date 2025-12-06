#pragma once
#include "Game/Classes/Players/CBaseEFTPlayer/CBaseEFTPlayer.h"

class CObservedPlayer : public CBaseEFTPlayer
{
private:
	uintptr_t m_PlayerControllerAddress{ 0 };
	uintptr_t m_MovementControllerAddress{ 0 };
	uintptr_t m_ObservedMovementStateAddress{ 0 };
	uintptr_t m_VoiceAddress{ 0 };
	wchar_t m_wVoice[32]{ 0 };

public:
	char m_Voice[32]{ 0 };

public:
	CObservedPlayer(uintptr_t EntityAddress) : CBaseEFTPlayer(EntityAddress) {}
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);

private:
	void SetSpawnTypeFromVoice();
};