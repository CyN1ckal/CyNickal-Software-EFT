/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Offsets.h"
#include "DMA/Pattern Scanner/Pattern Scanner.h"

void Offsets::ResolveAll(CProcess& Process) {
	auto& UnityPlayerModule = Process.GetModule("UnityPlayer.dll");

	Resolve_pGOM(UnityPlayerModule);
	Resolve_pCameras(UnityPlayerModule);

	auto& GameAssemblyModule = Process.GetModule("GameAssembly.dll");
	Resolve_pZLib(GameAssemblyModule);
}

void Offsets::Resolve_pGOM(CModule& UnityPlayer) {
	auto& Data = UnityPlayer.GetModuleAsBytes();
	const CPatternInfo GOMPattern("\x48\x89\x05????\x48\x83\xC4?\xC3\x33\xC9", "xxx????xxx?xxx");
	auto PatternOffset = PatternScanner::FindOffset(Data, GOMPattern);

	if (PatternOffset == std::numeric_limits<std::ptrdiff_t>::max()) {
		std::println("[Offsets/GOM] Failed to find pattern in data!");
		return;
	}

	uint32_t Displacement{ 0 };
	std::memcpy(&Displacement, &Data[PatternOffset + 3], sizeof(uint32_t));

	constexpr uint32_t INSTRUCTION_SIZE = 7;
	Offsets::pGOM = Displacement + PatternOffset + INSTRUCTION_SIZE;
	std::println("[Offsets/GOM] {0:X}", Offsets::pGOM);
}

void Offsets::Resolve_pCameras(CModule& UnityPlayer) {
	auto& Data = UnityPlayer.GetModuleAsBytes();
	const CPatternInfo CamerasPattern("\x4C\x8B\x05????\x33\xD2\x49\x8B\x48", "xxx????xxxxx");
	auto PatternOffset = PatternScanner::FindOffset(Data, CamerasPattern);

	if (PatternOffset == std::numeric_limits<std::ptrdiff_t>::max()) {
		std::println("[Offsets/Cameras] Failed to find pattern in data!");
		return;
	}

	uint32_t Displacement{ 0 };
	std::memcpy(&Displacement, &Data[PatternOffset + 3], sizeof(uint32_t));

	constexpr uint32_t INSTRUCTION_SIZE = 7;
	Offsets::pCameras = Displacement + PatternOffset + INSTRUCTION_SIZE;
	std::println("[Offsets/Cameras] {0:X}", Offsets::pCameras);
}

void Offsets::Resolve_pZLib(CModule& GameAssembly) {
	auto& Data = GameAssembly.GetModuleAsBytes();
	const CPatternInfo ZLibPattern("\x48\x8B\x05????\xBA????\x4C\x8B\x0D????\x41\xB8", "xxx????x????xxx????xx");
	auto PatternOffset = PatternScanner::FindOffset(Data, ZLibPattern);

	if (PatternOffset == std::numeric_limits<std::ptrdiff_t>::max()) {
		std::println("[Offsets/ZLib] Failed to find pattern in data!");
		return;
	}

	uint32_t Displacement{ 0 };
	std::memcpy(&Displacement, &Data[PatternOffset + 3], sizeof(uint32_t));

	constexpr uint32_t INSTRUCTION_SIZE{ 7 };
	Offsets::pZLib = Displacement + PatternOffset + INSTRUCTION_SIZE;
	std::println("[Offsets/ZLib] {0:X}", Offsets::pZLib);
}