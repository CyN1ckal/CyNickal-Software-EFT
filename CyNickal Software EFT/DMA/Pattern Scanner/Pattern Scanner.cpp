/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Pattern Scanner.h"

std::ptrdiff_t PatternScanner::FindOffset(const std::vector<char>& Data, const CPatternInfo& Pattern)
{
	const std::size_t BytesToSearch = Data.size() - Pattern.m_Mask.size();

	for (std::size_t OuterIdx = 0; OuterIdx < BytesToSearch; ++OuterIdx) {

		for (std::size_t PatternIndex = 0; PatternIndex < Pattern.m_Pattern.size(); ++PatternIndex) {

			if (!Pattern.Passes(PatternIndex, Data.at(OuterIdx + PatternIndex))) {
				break;
			}

			if (PatternIndex == (Pattern.m_Pattern.size() - 1)) {
				return OuterIdx;
			}
		}
	}

	return std::numeric_limits<std::ptrdiff_t>::max();
}