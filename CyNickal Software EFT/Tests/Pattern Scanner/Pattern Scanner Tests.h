/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "DMA/Pattern Scanner/Pattern Scanner.h"

TEST_CASE("Pattern Info") {
	CPatternInfo Pattern(std::string("01"), std::string("?x"));

	REQUIRE(Pattern.IsWildcard(0) == true);
	REQUIRE(Pattern.IsWildcard(1) == false);

	REQUIRE(Pattern.Passes(0, '0') == true);
	REQUIRE(Pattern.Passes(0, '1') == true);
	REQUIRE(Pattern.Passes(1, '1') == true);
	REQUIRE(Pattern.Passes(1, '0') == false);
}

TEST_CASE("Basic pattern match") {
	const std::vector<char> Data{ 0x1,0x2,0x3,0x4,0x5,0x6 };

	REQUIRE(PatternScanner::FindOffset(Data, CPatternInfo("\x01", "x")) == 0);
	REQUIRE(PatternScanner::FindOffset(Data, CPatternInfo("\x02", "x")) == 1);
	REQUIRE(PatternScanner::FindOffset(Data, CPatternInfo("\x03", "x")) == 2);
	REQUIRE(PatternScanner::FindOffset(Data, CPatternInfo("\x07", "x")) == std::numeric_limits<std::ptrdiff_t>::max());

	REQUIRE(PatternScanner::FindOffset(Data, CPatternInfo("1", "?")) == 0);
	REQUIRE(PatternScanner::FindOffset(Data, CPatternInfo("\x01\x02", "?x")) == 0);
	REQUIRE(PatternScanner::FindOffset(Data, CPatternInfo("\x02\x03", "?x")) == 1);
	REQUIRE(PatternScanner::FindOffset(Data, CPatternInfo("\x02\xFF", "?x")) == std::numeric_limits<std::ptrdiff_t>::max());
}