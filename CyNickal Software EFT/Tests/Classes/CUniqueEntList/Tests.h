/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "pch.h"
#include "Classes/CUniqueEntList/CUniqueEntList.h"

class CTestEnt {
public:
	uintptr_t m_EntAddress{ 0 };
	float Health{ 100.0f };

	constexpr bool operator==(const uintptr_t& Other) const {
		return m_EntAddress == Other;
	}
};

TEST_CASE("CTestEnt Equality Operators") {
	CTestEnt Ent1{ 0x1234 };
	CTestEnt Ent2{ 0x1234 };
	CTestEnt Ent3{ 0x5678 };
	REQUIRE(Ent1 == Ent2.m_EntAddress);
	REQUIRE(Ent1 != Ent3.m_EntAddress);
	REQUIRE(Ent1 == 0x1234);
	REQUIRE(Ent1 != 0x5678);
};

TEST_CASE("CUniqueEntList Empty On Init") {
	CUniqueEntList<CTestEnt> EntList{};
	REQUIRE(EntList.empty());
};

TEST_CASE("CUniqueEntList Basic Functionality") {
	std::vector<uintptr_t> EntAddresses{ 1,2,3,4,5,6,7,8 };

	/* allocate new entities */
	CUniqueEntList<CTestEnt> EntList{};
	EntList.HandleAllocations(EntAddresses);
	REQUIRE(EntList.size() == 8);
	REQUIRE(EntList.deallocCount() == 0);
	REQUIRE(EntList.allocCount() == 8);

	/* dont allocate duplicates */
	EntList.HandleAllocations(EntAddresses);
	REQUIRE(EntList.size() == 8);
	REQUIRE(EntList.deallocCount() == 0);
	REQUIRE(EntList.allocCount() == 8);

	/* erase old entities */
	EntAddresses.pop_back();
	EntAddresses.pop_back();
	EntList.HandleAllocations(EntAddresses);
	REQUIRE(EntList.size() == 6);
	REQUIRE(EntList.deallocCount() == 2);
	REQUIRE(EntList.allocCount() == 8);
};

TEST_CASE("CUniqueEntList HandleAllocations with empty vector") {
	std::vector<uintptr_t> EntAddresses{ };

	/* allocate new entities */
	CUniqueEntList<CTestEnt> EntList{};
	EntList.HandleAllocations(EntAddresses);
	REQUIRE(EntList.size() == 0);
	REQUIRE(EntList.deallocCount() == 0);
	REQUIRE(EntList.allocCount() == 0);
};

TEST_CASE("CUniqueEntList HandleAllocations With Duplicates") {
	std::vector<uintptr_t> EntAddresses{ 1,2,3,4,5,5,5,6,7,8 };
	/* allocate new entities */
	CUniqueEntList<CTestEnt> EntList{};
	EntList.HandleAllocations(EntAddresses);
	REQUIRE(EntList.size() == 8);
	REQUIRE(EntList.deallocCount() == 0);
	REQUIRE(EntList.allocCount() == 8);
};

TEST_CASE("CUniqueEntList ForEach Functionality") {
	std::vector<uintptr_t> EntAddresses{ 1,2,3,4,5 };
	CUniqueEntList<CTestEnt> EntList{};
	EntList.HandleAllocations(EntAddresses);

	uint32_t Sum{ 0 };

	EntList.ForEach([&Sum](const CTestEnt& Ent) {
		Sum += static_cast<uint32_t>(Ent.Health);
		});

	REQUIRE(Sum == 500);
};