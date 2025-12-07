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

	//4C 8B 05 ? ? ? ? 33 D2 49 8B 48
	//48 8B 05 ? ? ? ? 48 8B 08 49 8B 3C 0C
	//48 8B 05 ? ? ? ? 48 8B 38 48 8B 3C 3E
	//48 8B 05 ? ? ? ? 49 C7 C6 ? ? ? ? 8B 48 ? 85 C9 0F 84 ? ? ? ? 48 89 B4 24
	//48 8B 05 ? ? ? ? 49 C7 C6 ? ? ? ? 8B 48 ? 85 C9 0F 84 ? ? ? ? 48 89 9C 24
	inline constexpr std::ptrdiff_t pCameras{ 0x19F0100 };

	namespace CObject
	{
		inline constexpr std::ptrdiff_t pComponents{ 0x58 };
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
		inline constexpr std::ptrdiff_t pAiData{ 0x920 };
	}
	namespace CObservedPlayer
	{
		inline constexpr std::ptrdiff_t pPlayerBody{ 0xC8 };
		inline constexpr std::ptrdiff_t pPlayerController{ 0x20 };
		inline constexpr std::ptrdiff_t IsAi{ 0x98 };
		inline constexpr std::ptrdiff_t PlayerSide{ 0x8C };
		inline constexpr std::ptrdiff_t pVoice{ 0x38 };
		inline constexpr std::ptrdiff_t pAiData{ 0x68 };
	}
	namespace CPlayerController
	{
		inline constexpr std::ptrdiff_t pMovementController{ 0x30 };
	}
	namespace CMovementController
	{
		inline constexpr std::ptrdiff_t pObservedPlayerState{ 0x98 };
	}
	namespace CObservedMovementState
	{
		inline constexpr std::ptrdiff_t Rotation{ 0x20 };
	}
	namespace CMovementContext
	{
		inline constexpr std::ptrdiff_t Rotation{ 0xC8 };
	}
	namespace CPlayerBody
	{
		inline constexpr std::ptrdiff_t pSkeleton{ 0x30 };
	}
	namespace CCameras
	{
		inline constexpr std::ptrdiff_t pCameraList{ 0x0 };
		inline constexpr std::ptrdiff_t NumCameras{ 0x10 };
	}
	namespace CComponent
	{
		inline constexpr std::ptrdiff_t pGameObject{ 0x58 };
	}
	namespace CCamera
	{
		inline constexpr std::ptrdiff_t pCameraInfo{ 0x18 };
	}
	namespace CCameraInfo
	{
		inline constexpr std::ptrdiff_t Matrix{ 0x128 };
	}
	namespace CSkeleton
	{
		inline constexpr std::ptrdiff_t pSkeletonValues{ 0x30 };
	}
	namespace CValues
	{
		inline constexpr std::ptrdiff_t pArr1{ 0x10 };
	}
	namespace CTransformHierarchy
	{
		inline constexpr std::ptrdiff_t pVertices{ 0x8 };
		inline constexpr std::ptrdiff_t pIndices{ 0xA0 };

		inline constexpr std::ptrdiff_t Position{ 0xB0 };
	}
	namespace CObservedPlayerController
	{
		inline constexpr std::ptrdiff_t pMovementController{ 0xD8 };
	}
	namespace CAIData
	{
		inline constexpr std::ptrdiff_t pBotOwner{ 0x28 };
		inline constexpr std::ptrdiff_t bIsAi{ 0x100 };
	}
	namespace CProfile
	{
		inline constexpr std::ptrdiff_t pProfileInfo{ 0x48 };
	}
	namespace CProfileInfo
	{
		inline constexpr std::ptrdiff_t Side{ 0x48 };
	}
	namespace CBotOwner
	{
		inline constexpr std::ptrdiff_t pSpawnProfileData{ 0x3C0 };
	}
	namespace CSpawnProfileData
	{
		inline constexpr std::ptrdiff_t SpawnType{ 0x10 };
	}
	namespace CUnityTransform
	{
		inline constexpr std::ptrdiff_t pTransformHierarchy{ 0x70 };
		inline constexpr std::ptrdiff_t Index{ 0x78 };
	}
	namespace CBoneArray
	{
		inline constexpr std::ptrdiff_t ArrayStart{ 0x20 };
	}
	namespace CSkeletonValues
	{
		inline constexpr std::ptrdiff_t pBoneArray{ 0x10 };
	}
};