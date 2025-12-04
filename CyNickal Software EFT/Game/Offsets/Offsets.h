#pragma once
#include <cstddef>

namespace Offsets
{
	//48 89 05 ? ? ? ? 48 83 C4 ? C3 33 C9
	//48 8B 15 ? ? ? ? 48 83 C2 ? 48 3B DA
	//48 8B 35 ? ? ? ? 48 85 F6 0F 84 ? ? ? ? 8B 46
	//48 89 2D ? ? ? ? 48 8B 6C 24 ? 48 83 C4 ? 5E C3 33 ED
	//48 8B 0D ? ? ? ? 4C 8D 4C 24 ? 4C 8D 44 24 ? 89 44 24
	inline constexpr std::ptrdiff_t pGOM{ 0x1A208D8 };

	namespace CObject
	{
		inline constexpr std::ptrdiff_t pName{ 0x88 };
	}
	namespace CGameObjectManager
	{
		inline constexpr std::ptrdiff_t pActiveNodes{ 0x20 };
		inline constexpr std::ptrdiff_t pLastActiveNode{ 0x28 };
	};

	namespace CLocalGameWorld
	{
		inline constexpr std::ptrdiff_t pMapName{ 0xC0 };
		inline constexpr std::ptrdiff_t pRegisteredPlayers{ 0x198 };
		inline constexpr std::ptrdiff_t pMainPlayer{ 0x1E8 };
	};

	namespace CRegisteredPlayers
	{
		inline constexpr std::ptrdiff_t pPlayerArray{ 0x10 };
		inline constexpr std::ptrdiff_t NumPlayers{ 0x18 };
		inline constexpr std::ptrdiff_t MaxPlayers{ 0x1C };
	}

	namespace CPlayer
	{
		inline constexpr std::ptrdiff_t pMovementContext{ 0x60 };
		inline constexpr std::ptrdiff_t pPlayerBody{ 0x190 };
		inline constexpr std::ptrdiff_t pLocation{ 0x868 };
		inline constexpr std::ptrdiff_t pProfile{ 0x8E0 };
	}
	namespace CMovementContext
	{
		inline constexpr std::ptrdiff_t Rotation{ 0xC8 };
	}
	namespace CPlayerBody
	{
		inline constexpr std::ptrdiff_t pSkeleton{ 0x30 };
	}
};