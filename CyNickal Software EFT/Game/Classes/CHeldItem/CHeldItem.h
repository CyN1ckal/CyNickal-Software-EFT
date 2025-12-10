#pragma once
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Classes/CItem/CItem.h"
#include "Game/Enums/EPlayerType.h"

class CHeldItem : public CBaseEntity
{
public:
	CHeldItem(uintptr_t EntityAddress);
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh, EPlayerType PlayerType);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh, EPlayerType PlayerType);
	void Finalize();
	void QuickFinalize();
	void CompleteUpdate(EPlayerType PlayerType);

public:
	CItem m_HeldItem{ 0x0 };

private:
	uintptr_t m_HeldItemAddress{ 0 };
	uintptr_t m_PreviousHeldItemAddress{ 0 };
};