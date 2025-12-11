#pragma once

class CItemHash
{
public:
	CItemHash(const std::string& WeaponName);
	const char* GetName() const;
	const uint32_t GetHash() const { return m_Hash; }
	const bool IsValuable() const;

private:
	uint32_t m_Hash{ 0 };
};