#pragma once
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Classes/CUnityTransform/CUnityTransform.h"

class CLoot : public CBaseEntity
{
public:
	CLoot(uintptr_t LootAddress);
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	void QuickFinalize();

public:
	std::array<char, 64> m_Name{};
	Vector3 m_Position{};

private:
	CUnityTransform m_Transform{ 0x0 };
	uintptr_t m_DerefAddress{ 0 };
	uintptr_t m_MonoBehaviorAddress{ 0 };
	uintptr_t m_GameObjectAddress{ 0 };
	uintptr_t m_NameAddress{ 0 };
	uintptr_t m_ComponentAddress{ 0 };
	uintptr_t m_TransformAddress{ 0 };
};