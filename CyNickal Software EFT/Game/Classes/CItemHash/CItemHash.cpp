#include "pch.h"

#include "CItemHash.h"

std::unordered_map<uint32_t, std::string> gItemNames
{
	{0xBFA7DBD, "Saiga-12k"},
	{0x32142AA, "TOZ KS-23M"},
	{0xAE4D2212, "TOZ-106"},
	{0x68103A98, "MP-155"},
	{0xED31C50B, "MP-153"},
	{0x2E28AA9A, "MP-133"},
	{0x3A9D66E8, "Vepr"},
	{0xF89323EC, "Vepr-Hunter"},
	{0x2CA90C36, "Camper Axe"},
	{0xC3E4DC81, "AKS-74U"},
	{0x61961C9F, "Saiga-9"},
	{0xC95E7F32, "VPO-209"},
	{0xF8B8C6EA, "TT"},
	{0xE8BE68AA, "Desert Eagle"},
	{0x581508B6, "AK-74N"},
	{0x6CBC707F, "MP9"},
	{0x73B9DCD9, "SKS"},
	{0xD9C5BAD4, "MP-443 Grach"},
	{0xEAA0376F, "RPK-16"},
	{0x2055395E, "OP-SKS"},
	{0x73EE1B07, "AK-74M"},
	{0xCF7C4C8F, "AK-105"},
	{0xC7D1A827, "ADAR 2"},
	{0xAC676528, "Salewa"},
};

uint32_t JOAAT(const std::string& String) {
	size_t i = 0;
	uint32_t hash = 0;
	while (i != String.size()) {
		if (String[i] == '\0') break;
		hash += String[i++];
		hash += hash << 10;
		hash ^= hash >> 6;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
	return hash;
}

CItemHash::CItemHash(const std::string& WeaponName)
{
	m_Hash = JOAAT(WeaponName);

	if (m_Hash)
		std::println("[CItemHash] Generated hash {0:X} for weapon name {1}", m_Hash, std::string(WeaponName.begin(), WeaponName.end()).c_str());
}

const std::string& CItemHash::GetName() const
{
	if (gItemNames.find(m_Hash) != gItemNames.end())
		return gItemNames.at(m_Hash);

	static std::string Unknown = "Unknown";

	return Unknown;
}