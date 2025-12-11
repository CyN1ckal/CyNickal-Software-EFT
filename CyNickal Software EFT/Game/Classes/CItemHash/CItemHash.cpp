#include "pch.h"

#include "CItemHash.h"

const std::unordered_map<uint32_t, std::string> gItemNames
{
	{0xBFA7DBD, "Saiga-12k"},
	{0x32142AA, "TOZ KS-23M"},
	{0xAE4D2212, "TOZ-106"},
	{0x68103A98, "MP-155"},
	{0xED31C50B, "MP-153"},
	{0xA10D4A9F, "MP-133"},
	{0x3A9D66E8, "Vepr"},
	{0xF89323EC, "Vepr-Hunter"},
	{0x2CA90C36, "Camper Axe"},
	{0xC3E4DC81, "AKS-74U"},
	{0x61961C9F, "Saiga-9"},
	{0x1053DA5A, "Saiga-12 FA"},
	{0xC95E7F32, "VPO-209"},
	{0x905A34FF, "PM"},
	{0x065CEB7C, "TT"},
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
	{0x095614E9, "M1A"},
	{0x88A5F812, "NL545"},
	{0xF1B44B48, "NL545"},
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
	{0x7CCD75F2, "MXLR"},
	{0x98AFA2DC, "AKS-74"},
	{0x98879D3B, "SVD"},
	{0x640DFFA2, "SR-25"},
	{0xAEF352D8, "HK G36"},
	{0xAE1830F2, "AK-545 Short"},
	{0x7AEE21CD, "SV-98"},
	{0x90DBD156, "AVT-40"},
	{0xA7A0A474, "Rsh-12"},
	{0xE9FB687D, "RFB"},
	{0xA64E7E41, "AA-12"},
	{0x4AC6DB7F, "MCX .300"},
	{0x36F96D5A, "FN-SCAR"},
	{0x1173B972, "RSASS"},
	{0xA3337E07, "AK-103"},
	{0x7DD65260, "IWI UZI"},
	{0x9A6EDF14, "PPSh-41"},
	{0xFB2CD802, "M4A1"},
	{0x71A0A377, "M16A1"},

	/* Other items */
	{0x7107CDBF, "Power Cord"},
	{0x690F712C, "Pliers"},
	{0xD661A199, "Elite Pliers"},
	{0x2BCECDE, "Propane"},
	{0x14698670, "RFID Reader"},
	{0x1102B85B, "Tushonka"},
	{0xE80284D2, "Salewa"},
	{0xCA057ECE, "PSU"},
	{0xD2D7055D, "Emelya"},
	{0x6F1A0D0B, "Energy Drink"},
	{0xDDCFFAD3, "AA Batteries"},
	{0x4F7BA371, "Drill"},
	{0x772D13F3, "Toothpaste"},
	{0x70469D50, "Chlorine"},

	/* Containers */
	{0x1D2E27D9, "PC Block"},
	{0x1D3ADFCA, "Toolbox"},
	{0xD328FEF6, "Sportbag"},
	{0xB3E04EE3, "Jacket"},
	{0x2C865C85, "Jacket"},
	{0x9082B9FB, "Jacket"},
	{0xAC53E66B, "Jacket"},
	{0x19BE107F, "Filing Cabinet"},
	{0xF8CFC1D, "Filing Cabinet"},
	{0x1365F70, "Filing Cabinet"},
	{0x37154B39, "Filing Cabinet"},
	{0x8F9E34D, "Static Corpse"},
	{0xFE8849CF, "Static Corpse"},
	{0x71DF944E, "Static Corpse"},
	{0x3B201746, "Static Corpse"},
	{0xA9904ABD, "Ammo Crate"},
	{0x84CAE97A, "Weapon Box"},
};

const std::unordered_map<uint32_t, std::string> gValuables
{
	{0x627F6841, "Bitcoin"},
	{0x281E0D2C, "Golden Egg"},
	{0x9FDB5E61, "Sledgehammer"},
	{0x32A54032, "Intelligence"},
	{0xCDF181D5, "Cat"},
	{0x2A9827C2, "Parrot"},
	{0x360C4F1F, "Teapot"},
	{0x1C8CC38B, "Skullring"},
	{0xA7D5AD85, "Kunai"},
	{0x8443B372, "Medallion"},
	{0xF4FEF1E3, "Lion"},
	{0x90C1DB5E, "Rolex"},
	{0x3588C73A, "GP"},
	{0x8A7E0181, "Vase"},
	{0x8DAB3325, "Chicken"},
	{0x2475DDCB, "Raven"},
	{0xAC48899A, "Badge"},
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

CItemHash::CItemHash(const std::string& ItemName)
{
	m_Hash = JOAAT(ItemName);

	if (m_Hash)
		std::println("[CItemHash] Generated hash {0:X} for item name {1}", m_Hash, std::string(ItemName.begin(), ItemName.end()).c_str());
}

const char* CItemHash::GetName() const
{
	if (gItemNames.find(m_Hash) != gItemNames.end())
		return gItemNames.at(m_Hash).c_str();

	if (gValuables.find(m_Hash) != gValuables.end())
		return gValuables.at(m_Hash).c_str();

	return nullptr;
}

const bool CItemHash::IsValuable() const
{
	return gValuables.find(m_Hash) != gValuables.end();
}