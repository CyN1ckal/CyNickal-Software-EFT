/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once

class CBaseEntity
{
public:
	uintptr_t m_EntityAddress{ 0 };
	uint32_t m_BytesRead{ 0 };
	uint8_t m_Flags{ 0 };

public:
	CBaseEntity(uintptr_t EntityAddress) : m_EntityAddress(EntityAddress) {
		if (!m_EntityAddress)
			SetInvalid();
	}
	~CBaseEntity() = default;
	CBaseEntity(CBaseEntity&& Mov) = default;
	CBaseEntity(const CBaseEntity& Cpy) = default;
	CBaseEntity& operator=(const CBaseEntity& Other) = default;
	CBaseEntity& operator=(CBaseEntity&& Move) = default;

	void SetInvalid();
	bool IsInvalid() const;
	bool operator==(const CBaseEntity& other) const;
	uintptr_t GetAddress() const { return m_EntityAddress; }
};