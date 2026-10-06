/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Classes/CItemTemplate/CItemTemplate.h"

struct CShallowMagazine
{
	std::string m_AmmoTypeName{};
	uint32_t m_CurrentCartridges{ 0 };
	uint32_t m_MaxCartridges{ 0 };
};

class CMagazine : public CBaseEntity
{
public:
	CMagazine(uintptr_t MagazineSlotAddress);

	~CMagazine() = default;
	CMagazine(CMagazine&&) = default;
	CMagazine(const CMagazine&) = default;
	CMagazine& operator=(CMagazine&&) = default;
	CMagazine& operator=(const CMagazine&) = default;

	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickFinalize();

	const std::string& GetAmmoName() const;
	const CShallowMagazine ShallowCopy() const;

public:
	uint32_t m_MaxCartridges{ 0 };
	uint32_t m_CurrentCartridges{ 0 };

	std::optional<CItemTemplate> m_pMagazineItemTemplate{ std::nullopt };
	std::optional<CItemTemplate> m_pAmmoItemTemplate{ std::nullopt };

private:
	std::string m_AmmoName{ "" };
	uintptr_t m_ContainedItemAddress{ 0 };
	uintptr_t m_MagazineCartridgesAddress{ 0 };
	uintptr_t m_StackItemsAddress{ 0 };
	uintptr_t m_ListAddress{ 0 };
	uintptr_t m_AmmoItemAddress{ 0 };
	uintptr_t m_AmmoTemplateAddress{ 0 };
	uintptr_t m_MagazineTemplateAddress{ 0 };
};