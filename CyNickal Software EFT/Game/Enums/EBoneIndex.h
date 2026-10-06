/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include <cstdint>

enum class EBoneIndex : uint32_t
{
	Root = 0,
	Pelvis = 14,
	Head = 133,
	Neck = 132,
	Spine3 = 37,
	LThigh1 = 15,
	LThigh2 = 16,
	LCalf = 17,
	LFoot = 18,
	RThigh1 = 20,
	RThigh2 = 21,
	RCalf = 22,
	RFoot = 23,
	LUpperArm = 90,
	LForeArm1 = 91,
	LForeArm2 = 92,
	LPalm = 94,
	RUpperArm = 111,
	RForeArm1 = 112,
	RForeArm2 = 113,
	RPalm = 115,
};