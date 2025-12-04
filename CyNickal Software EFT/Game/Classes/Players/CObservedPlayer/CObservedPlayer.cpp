#include "pch.h"
#include "CObservedPlayer.h"

void CObservedPlayer::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_1(vmsh, EPlayerType::eObservedPlayer);
}