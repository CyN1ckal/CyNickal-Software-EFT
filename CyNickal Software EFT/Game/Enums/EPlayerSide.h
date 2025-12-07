#pragma once
#include <cstdint>
#include <limits>

enum class EPlayerSide : uint32_t
{
	USEC = 0,
	BEAR = 2,
	SCAV = 4,
	UNKNOWN = std::numeric_limits<uint32_t>::max()
};