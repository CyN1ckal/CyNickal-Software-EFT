#pragma once
#include "Game/Enums/EExfilStatus.h"
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Classes/CUnityTransform/CUnityTransform.h"

class CExfilPoint : public CBaseEntity
{
public:
	CExfilPoint(uintptr_t ExfilPointAddress);
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	const ImColor& GetRadarColor() const;
	const ImColor& GetFuserColor() const;

public:
	Vector3 m_Position{};
	EExfilStatus m_Status{ EExfilStatus::UNKNOWN };
	std::string m_Name{};

private:
	std::array<char, 64> m_NameBuffer{};
	CUnityTransform m_Transform{ 0x0 };
	uintptr_t m_ComponentAddress{ 0x0 };
	uintptr_t m_GameObjectAddress{ 0x0 };
	uintptr_t m_NameAddress{ 0x0 };
	uintptr_t m_ComponentsAddress{ 0x0 };
	uintptr_t m_TransformAddress{ 0x0 };
};
