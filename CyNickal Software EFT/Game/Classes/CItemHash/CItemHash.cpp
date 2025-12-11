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
	{0x1053DA5A, "Saiga-12 FA"},
	{0xC95E7F32, "VPO-209"},
	{0x905A34FF, "PM"},
	{0x65CEB7C, "TT"},
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
	{0xA967B8A6, "PP-91"},
	{0x382F41EC, "590A1"},
	{0x8A20059A, "Milk"},
	{0x2110A498, "MP-443"},
	{0xEFE162B5, "AK-104"},
	{0x95614E9, "M1A"},
	{0x88A5F812, "NL545"},
	{0xB2F3D739, "MP9-N"},
	{0xC4C341F6, "AK-545"},
	{0x5D2895D3, "Mosin Infantry"},
	{0x463C2B8C, "Mosin"},
	{0xF31A4204, "AKMS"},
	{0xAED7C176, "PKTM"},
	{0x89D12ECF, "Aquamari"},
	{0x975C3D06, "M67"},
	{0x55D5BA0F, "Splint+"},
	{0x9c5b7355, "Splint"},
	{0xde317059, "Grizzly"},
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