#include "pch.h"
#include "CObservedPlayer.h"
#include "Game/Offsets/Offsets.h"

void CObservedPlayer::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEFTPlayer::PrepareRead_1(vmsh, EPlayerType::eObservedPlayer);

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CObservedPlayer::IsAi, sizeof(std::byte), reinterpret_cast<BYTE*>(&m_IsAiByte), nullptr);
}

const bool CObservedPlayer::IsAi() const
{
	return m_IsAiByte != std::byte{ 0 };
}