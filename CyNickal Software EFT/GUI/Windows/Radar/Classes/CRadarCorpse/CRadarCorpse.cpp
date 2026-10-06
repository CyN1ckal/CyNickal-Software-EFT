/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CRadarCorpse.h"
#include "GUI/Windows/Color Picker/Color Picker.h"

CRadarCorpse::CRadarCorpse(const CCorpse& Corpse) : CRadarHoverable(Corpse.m_Position, ColorPicker::Radar::m_CorpseColor) {
}

bool CRadarCorpse::Draw(ImDrawList* DrawList) const
{
	auto bHovered = CRadarHoverable::Draw(DrawList);

	return bHovered;
}