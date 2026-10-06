/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once

class CPatternInfo {
public:
	CPatternInfo(const std::string& Pattern, const std::string& Mask) {
		if (Pattern.size() != Mask.size())
			throw std::runtime_error("[CPatternInfo] Pattern and mask must be same size!");

		if (Pattern.empty())
			throw std::runtime_error("CPatternInfo] Pattern and mask cannot be empty!");

		m_Pattern = Pattern;
		m_Mask = Mask;
	}

public:
	std::string m_Pattern{};
	std::string m_Mask{};

public:
	bool IsWildcard(std::size_t Index) const {
		if (m_Mask.at(Index) == '?')
			return true;

		return false;
	}

	bool Passes(std::size_t PatternIndex, const char ComparisonByte) const {
		if (IsWildcard(PatternIndex))
			return true;

		if (m_Pattern.at(PatternIndex) == ComparisonByte)
			return true;

		return false;
	}
};

namespace PatternScanner {
	std::ptrdiff_t FindOffset(const std::vector<char>& Data, const CPatternInfo& Pattern);
}