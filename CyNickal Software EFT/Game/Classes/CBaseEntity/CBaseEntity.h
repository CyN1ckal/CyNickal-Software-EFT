#pragma once

class CBaseEntity
{
public:
	uintptr_t m_EntityAddress{ 0 };
	uint32_t m_BytesRead{ 0 };
	uint8_t m_Flags{ 0 };

public:
	CBaseEntity(uintptr_t EntityAddress);
	void SetInvalid();
	bool IsInvalid() const;
	bool operator==(const CBaseEntity& other) const;
	uintptr_t GetAddress() const { return m_EntityAddress; }
};