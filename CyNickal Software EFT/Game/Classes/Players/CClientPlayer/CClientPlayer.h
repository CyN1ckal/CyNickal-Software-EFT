#pragma once
#include "Game/Classes/Players/CBaseEFTPlayer/CBaseEFTPlayer.h"

class CClientPlayer : public CBaseEFTPlayer
{
private:
	uintptr_t m_MovementContextAddress{ 0 };
	uintptr_t m_ProfileAddress{ 0 };
	uintptr_t m_ProfileInfoAddress{ 0 };
	uintptr_t m_HandsControllerAddress{ 0 };
	uintptr_t m_PreviousHandsControllerAddress{ 0 };
	uintptr_t m_ProceduralWeaponAnimationAddress{ 0 };
	uintptr_t m_OpticsAddress{ 0 };
	std::byte m_AimingByte{ 0 };

public:
	CClientPlayer(uintptr_t EntityAddress) : CBaseEFTPlayer(EntityAddress) {}
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_9(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_10(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_11(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_12(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_13(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_14(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	void QuickFinalize();
	bool IsAiming() const { return m_AimingByte != std::byte{ 0 }; }
};