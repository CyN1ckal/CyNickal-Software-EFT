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
#include "Game/Enums/EAllocationType.h"
class CItem : public CBaseEntity
{
public:
	CItem(uintptr_t EntityAddress, EAllocationType AllocType = EAllocationType::EMPTY);
	~CItem() = default;
	CItem(const CItem&) = default;
	CItem(CItem&&) = default;
	CItem& operator=(const CItem&) = default;
	CItem& operator=(CItem&&) = default;

	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void CompleteUpdate();
	void Finalize();

	const std::string& GetItemName() const;

public:
	std::optional<CItemTemplate> m_pItemTemplate{ std::nullopt };

private:
	std::string m_ItemName{ "" };
	uintptr_t m_ItemTemplateAddress{ 0 };
};