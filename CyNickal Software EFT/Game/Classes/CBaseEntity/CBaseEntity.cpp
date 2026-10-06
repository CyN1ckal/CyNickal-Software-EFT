/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CBaseEntity.h"

void CBaseEntity::SetInvalid()
{
	m_Flags |= 0x1;
}

bool CBaseEntity::IsInvalid() const
{
	return m_Flags & 0x1;
}

bool CBaseEntity::operator==(const CBaseEntity& other) const
{
	return other.m_EntityAddress == m_EntityAddress;
}