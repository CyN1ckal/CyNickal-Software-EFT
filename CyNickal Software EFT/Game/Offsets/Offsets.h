#pragma once
#include <cstddef>

namespace Offsets
{
	// UnityPlayer.dll
	//48 89 05 ? ? ? ? 48 83 C4 ? C3 33 C9
	//48 8B 15 ? ? ? ? 48 83 C2 ? 48 3B DA
	//48 8B 35 ? ? ? ? 48 85 F6 0F 84 ? ? ? ? 8B 46
	//48 89 2D ? ? ? ? 48 8B 6C 24 ? 48 83 C4 ? 5E C3 33 ED
	//48 8B 0D ? ? ? ? 4C 8D 4C 24 ? 4C 8D 44 24 ? 89 44 24
	inline constexpr std::ptrdiff_t pGOM{ 0x1A233A0 };

	// UnityPlayer.dll
	//4C 8B 05 ? ? ? ? 33 D2 49 8B 48
	//48 8B 05 ? ? ? ? 48 8B 08 49 8B 3C 0C
	//48 8B 05 ? ? ? ? 48 8B 38 48 8B 3C 3E
	//48 8B 05 ? ? ? ? 49 C7 C6 ? ? ? ? 8B 48 ? 85 C9 0F 84 ? ? ? ? 48 89 B4 24
	//48 8B 05 ? ? ? ? 49 C7 C6 ? ? ? ? 8B 48 ? 85 C9 0F 84 ? ? ? ? 48 89 9C 24
	inline constexpr std::ptrdiff_t pCameras{ 0x19F3080 };

	// GameAssembly.dll
	// 48 8B 0D ? ? ? ? 8B F0 48 8B 91 ? ? ? ? 48 8B 4A
	// 48 8B 05 ? ? ? ? BA ? ? ? ? 4C 8B 0D ? ? ? ? 41 B8
	// 48 8B 05 ? ? ? ? 48 8B 80 ? ? ? ? 48 89 6C 24 ? 4C 8B 30
	// 48 8B 0D ? ? ? ? 48 8B 89 ? ? ? ? 48 89 6C 24 ? 4C 8B 31
	// 48 8B 0D ? ? ? ? 48 8B F8 48 8B 91 ? ? ? ? 48 8B 0A 48 85 C9 74
	inline constexpr std::ptrdiff_t ZLibObject{ 0x58102F8 };

	namespace CGameObjectManager
	{
		inline constexpr std::ptrdiff_t pActiveNodes{ 0x20 };
		inline constexpr std::ptrdiff_t pLastActiveNode{ 0x28 };
	};

	/* namespace: EFT, class: GameWorld : UnityEngine::MonoBehaviour */
	namespace CLocalGameWorld
	{
		inline constexpr std::ptrdiff_t pExfiltrationController{ 0x50 };
		inline constexpr std::ptrdiff_t pMapName{ 0xC8 };
		inline constexpr std::ptrdiff_t pLootList{ 0x190 };
		inline constexpr std::ptrdiff_t pRegisteredPlayers{ 0x1B0 };
		inline constexpr std::ptrdiff_t pMainPlayer{ 0x208 };
	};

	namespace CExfiltrationController
	{
		inline constexpr std::ptrdiff_t pExfiltrationPoints{ 0x20 };
	};

	namespace CGenericList
	{
		inline constexpr std::ptrdiff_t Num{ 0x18 };
		inline constexpr std::ptrdiff_t StartData{ 0x20 };
	}

	namespace CExfiltrationPoint
	{
		inline constexpr std::ptrdiff_t pUnknown{ 0x10 };
		inline constexpr std::ptrdiff_t ExfilStatus{ 0x58 };
	}

	namespace CComponents
	{
		inline constexpr std::ptrdiff_t pTransform{ 0x8 };
	}

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
		inline constexpr std::ptrdiff_t pProceduralWeaponAnimation{ 0x338 };
		inline constexpr std::ptrdiff_t pProfile{ 0x900 };
		inline constexpr std::ptrdiff_t pAiData{ 0x940 };
		inline constexpr std::ptrdiff_t pHandsController{ 0x980 };
	}
	namespace CProceduralWeaponAnimation
	{
		inline constexpr std::ptrdiff_t pOptics{ 0x180 };
		inline constexpr std::ptrdiff_t bAiming{ 0x145 };
	}
	namespace CObservedPlayer
	{
		inline constexpr std::ptrdiff_t pPlayerBody{ 0xD8 };
		inline constexpr std::ptrdiff_t pPlayerController{ 0x28 };
		inline constexpr std::ptrdiff_t IsAi{ 0xA0 };
		inline constexpr std::ptrdiff_t PlayerSide{ 0x94 };
		inline constexpr std::ptrdiff_t pVoice{ 0x40 };
		inline constexpr std::ptrdiff_t pAiData{ 0x70 };
	}
	namespace CPlayerController
	{
		inline constexpr std::ptrdiff_t pMovementController{ 0x30 };
		inline constexpr std::ptrdiff_t pHands{ 0x30 };
	}
	namespace CMovementController
	{
		inline constexpr std::ptrdiff_t pObservedPlayerState{ 0x98 };
	}
	namespace CObservedMovementState
	{
		inline constexpr std::ptrdiff_t Rotation{ 0x20 };
		inline constexpr std::ptrdiff_t pObservedPlayerHands{ 0x130 };
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
		inline constexpr std::ptrdiff_t pObjectClass{ 0x20 };
		inline constexpr std::ptrdiff_t pGameObject{ 0x58 };
	}
	namespace CCamera
	{
		inline constexpr std::ptrdiff_t pCameraInfo{ 0x18 };
	}
	namespace CCameraInfo
	{
		inline constexpr std::ptrdiff_t Zoom{ 0xE8 };
		inline constexpr std::ptrdiff_t Matrix{ 0x128 };
		inline constexpr std::ptrdiff_t FOV{ 0x1A8 };
		inline constexpr std::ptrdiff_t AspectRatio{ 0x518 };
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
		inline constexpr std::ptrdiff_t pVertices{ 0x68 };
		inline constexpr std::ptrdiff_t pIndices{ 0x40 };
	}
	namespace CObservedPlayerController
	{
		inline constexpr std::ptrdiff_t pMovementController{ 0xD8 };
		inline constexpr std::ptrdiff_t pHealthController{ 0xE8 };
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
		inline constexpr std::ptrdiff_t pSpawnProfileData{ 0x3C8 };
	}

	/* EFT.InventoryLogic::StackSlot */
	namespace CStackSlot
	{
		inline constexpr std::ptrdiff_t Max{ 0x10 };
		inline constexpr std::ptrdiff_t pItems{ 0x18 };
	}

	/* EFT.InventoryLogic::Slot */
	namespace CSlot
	{
		inline constexpr std::ptrdiff_t pContainedItem{ 0x48 };
	}

	/* namespace: EFT.InventoryLogic, class: Item : System::Object */
	namespace CItem
	{
		inline constexpr std::ptrdiff_t StackCount{ 0x24 };
		inline constexpr std::ptrdiff_t pTemplate{ 0x60 };
		inline constexpr std::ptrdiff_t pMagslot{ 0xC8 };
		inline constexpr std::ptrdiff_t pCartridges{ 0xA8 };
	}

	/* EFT::InventoryLogic::ItemTemplate */
	namespace CItemTemplate
	{
		inline constexpr std::ptrdiff_t pShortName{ 0x18 };
		inline constexpr std::ptrdiff_t pDescription{ 0x20 };
		inline constexpr std::ptrdiff_t pTarkovID{ 0xF0 };
		inline constexpr std::ptrdiff_t pName{ 0xF8 };
		inline constexpr std::ptrdiff_t Width{ 0x3C };
		inline constexpr std::ptrdiff_t Height{ 0x40 };
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
	namespace CGameObject
	{
		inline constexpr std::ptrdiff_t pComponents{ 0x58 };
		inline constexpr std::ptrdiff_t pName{ 0x88 };
	}
	namespace CHealthController
	{
		inline constexpr std::ptrdiff_t HealthStatus{ 0x10 };
	}

	/*namespace: , class: ItemHandsController : AbstractHandsController */
	namespace CHandsController
	{
		inline constexpr std::ptrdiff_t pItem{ 0x70 };
	}
	/* namespace: EFT.NextObservedPlayer, class: ObservedPlayerHandsController : System::Object */
	namespace CObservedPlayerHands
	{
		inline constexpr std::ptrdiff_t pItem{ 0x58 };
	}

	namespace CMonoBehavior
	{
		inline constexpr std::ptrdiff_t pGameObject{ 0x58 };
	}

	/* EFT.Interactive::LootItem */
	namespace CLootItem
	{
		inline constexpr std::ptrdiff_t pTemplateID{ 0x80 };
		inline constexpr std::ptrdiff_t pItem{ 0xF0 };
	}

	/* EFT.Interactive::LootableContainer */
	namespace CLootableContainer
	{
		inline constexpr std::ptrdiff_t pBSGID{ 0x170 };
	}
};