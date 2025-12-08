#include "pch.h"
#include "CBaseEntity.h"

CBaseEntity::CBaseEntity(uintptr_t EntityAddress)
	: m_EntityAddress(EntityAddress)
{
	if (!m_EntityAddress)
		SetInvalid();
}

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