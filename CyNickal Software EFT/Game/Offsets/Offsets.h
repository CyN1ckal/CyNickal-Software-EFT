/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include <cstddef>
#include "DMA/CProcess/CProcess.h"

namespace Offsets
{
	void ResolveAll(CProcess& Process);

	// UnityPlayer.dll
	//48 89 05 ? ? ? ? 48 83 C4 ? C3 33 C9
	//48 8B 15 ? ? ? ? 48 83 C2 ? 48 3B DA
	//48 8B 35 ? ? ? ? 48 85 F6 0F 84 ? ? ? ? 8B 46
	//48 89 2D ? ? ? ? 48 8B 6C 24 ? 48 83 C4 ? 5E C3 33 ED
	//48 8B 0D ? ? ? ? 4C 8D 4C 24 ? 4C 8D 44 24 ? 89 44 24
	inline std::ptrdiff_t pGOM{ 0x1A233A0 };
	void Resolve_pGOM(CModule& UnityPlayer);

	// UnityPlayer.dll
	//4C 8B 05 ? ? ? ? 33 D2 49 8B 48
	//48 8B 05 ? ? ? ? 48 8B 08 49 8B 3C 0C
	//48 8B 05 ? ? ? ? 48 8B 38 48 8B 3C 3E
	//48 8B 05 ? ? ? ? 49 C7 C6 ? ? ? ? 8B 48 ? 85 C9 0F 84 ? ? ? ? 48 89 B4 24
	//48 8B 05 ? ? ? ? 49 C7 C6 ? ? ? ? 8B 48 ? 85 C9 0F 84 ? ? ? ? 48 89 9C 24
	inline std::ptrdiff_t pCameras{ 0x19F3080 };
	void Resolve_pCameras(CModule& UnityPlayer);

	// GameAssembly.dll
	// 48 8B 0D ? ? ? ? 8B F0 48 8B 91 ? ? ? ? 48 8B 4A
	// 48 8B 05 ? ? ? ? BA ? ? ? ? 4C 8B 0D ? ? ? ? 41 B8
	// 48 8B 05 ? ? ? ? 48 8B 80 ? ? ? ? 48 89 6C 24 ? 4C 8B 30
	// 48 8B 0D ? ? ? ? 48 8B 89 ? ? ? ? 48 89 6C 24 ? 4C 8B 31
	// 48 8B 0D ? ? ? ? 48 8B F8 48 8B 91 ? ? ? ? 48 8B 0A 48 85 C9 74
	inline std::ptrdiff_t pZLib{ 0x5934738 };
	void Resolve_pZLib(CModule& GameAssembly);

	namespace CGameObjectManager
	{
		inline constexpr std::ptrdiff_t pActiveNodes{ 0x20 };
		inline constexpr std::ptrdiff_t pLastActiveNode{ 0x28 };
	};

	/* namespace: EFT, class: GameWorld : UnityEngine::MonoBehaviour */
	namespace CLocalGameWorld
	{
		inline constexpr std::ptrdiff_t pExfiltrationController{ 0x58 };
		inline constexpr std::ptrdiff_t pMapName{ 0xD0 };
		inline constexpr std::ptrdiff_t pLootList{ 0x198 };
		inline constexpr std::ptrdiff_t pRegisteredPlayers{ 0x1B8 };
		inline constexpr std::ptrdiff_t pMainPlayer{ 0x210 };
		inline constexpr std::ptrdiff_t pGrenades{ 0x288 };
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
		inline constexpr std::ptrdiff_t pBSGId{ 0x30 };
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
		inline constexpr std::ptrdiff_t pCorpse{ 0x680 };
		inline constexpr std::ptrdiff_t pProfile{ 0x900 };
		inline constexpr std::ptrdiff_t pAiData{ 0x940 };
		inline constexpr std::ptrdiff_t pHandsController{ 0x980 };
		inline constexpr std::ptrdiff_t pPhysical{ 0x918 };
	}
	namespace CProceduralWeaponAnimation
	{
		inline constexpr std::ptrdiff_t pOptics{ 0x180 };
		inline constexpr std::ptrdiff_t bAiming{ 0x145 };
		inline constexpr std::ptrdiff_t ShotDirection{ 0x1C8 };
		inline constexpr std::ptrdiff_t fAimSwayStrength{ 0x27C };
		inline constexpr std::ptrdiff_t fAimSwayStartThreshold{ 0x274 };
		inline constexpr std::ptrdiff_t fAimSwayMaxThreshold{ 0x278 };
		inline constexpr std::ptrdiff_t AimSwayDirection{ 0x280 };
		inline constexpr std::ptrdiff_t fSwayStrength{ 0x390 };
		inline constexpr std::ptrdiff_t bShotNeedsFovAdjustments{ 0x433 };
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
		inline constexpr std::ptrdiff_t pQuests{ 0x98 };
	}
	namespace CProfileInfo
	{
		inline constexpr std::ptrdiff_t Side{ 0x48 };
	}
	namespace CBotOwner
	{
		inline constexpr std::ptrdiff_t pSpawnProfileData{ 0x3D0 };
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
		inline constexpr std::ptrdiff_t bQuestItem{ 0x34 };
		inline constexpr std::ptrdiff_t Width{ 0x3C };
		inline constexpr std::ptrdiff_t Height{ 0x40 };
		inline constexpr std::ptrdiff_t pTarkovID{ 0xF0 };
		inline constexpr std::ptrdiff_t pName{ 0xF8 };
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
		inline constexpr std::ptrdiff_t pCorpse{ 0x20 };
	}

	/*namespace: , class: ItemHandsController : AbstractHandsController */
	namespace CHandsController
	{
		inline constexpr std::ptrdiff_t pItem{ 0x70 };
	}

	/* [Class] EFT.ClientFirearmController : FirearmController : ItemHandsController */
	namespace CFirearmController {
		inline constexpr std::ptrdiff_t fCenterOfImpact{ 0xF0 };
		inline constexpr std::ptrdiff_t pFireport{ 0x150 };
		inline constexpr std::ptrdiff_t fHipInaccuracy{ 0x168 };
		inline constexpr std::ptrdiff_t LastShotId{ 0x438 };
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
		inline constexpr std::ptrdiff_t pBSGId{ 0x80 };
		inline constexpr std::ptrdiff_t pItem{ 0xF0 };
	}

	/* EFT.Interactive::LootableContainer */
	namespace CLootableContainer
	{
		inline constexpr std::ptrdiff_t pBSGID{ 0x170 };
	}

	namespace CUnityList {
		constexpr std::ptrdiff_t ArrayOffset = 0x10;
		constexpr std::ptrdiff_t Count = 0x18;
		constexpr std::ptrdiff_t ArrStartOffset = 0x20;
	}

	namespace CQuestEntry {
		inline constexpr std::ptrdiff_t pBSGId{ 0x10 };
		inline constexpr std::ptrdiff_t Status{ 0x1C };
		inline constexpr std::ptrdiff_t pCompletedConditions{ 0x28 };
	}

	// LevelSettings : UnityEngine.MonoBehaviour
	namespace CLevelSettings {
		inline constexpr std::ptrdiff_t AmbientMode{ 0x60 };
		inline constexpr std::ptrdiff_t EquatorColor{ 0x74 };
		inline constexpr std::ptrdiff_t GroundColor{ 0x84 };
	}

	namespace CComponentTypeInfo
	{
		inline constexpr std::ptrdiff_t pName{ 0x10 };
	}

	// [Class] TOD_Sky : MonoBehaviourSingleton`1
	namespace CTODSky {
		inline constexpr std::ptrdiff_t pCycle{ 0x38 };
	}

	// [Class] Cycle : System.Object
	namespace CCycle {
		inline constexpr std::ptrdiff_t CycleSpeed{ 0x10 };
		inline constexpr std::ptrdiff_t Time{ 0x24 };
	}
	
	// [Class] TOD_Time : UnityEngine.MonoBehaviour
	namespace CTODTime {
		inline constexpr std::ptrdiff_t bLockCurrentTime{ 0x20 };
	}

	namespace CWeatherController {
		inline constexpr std::ptrdiff_t pWeatherDebug{ 0x88 };
	}

	namespace CWeatherDebug {
		inline constexpr std::ptrdiff_t bEnabled{ 0x10 };
		inline constexpr std::ptrdiff_t fCloudDensity{ 0x24 };
		inline constexpr std::ptrdiff_t fFog{ 0x28 };
		inline constexpr std::ptrdiff_t fRain{ 0x2C };
	}

	namespace CPhysical {
		inline constexpr std::ptrdiff_t pStamina{ 0x68 };
	}

	namespace CPhysicalValue {
		inline constexpr std::ptrdiff_t Current{ 0x10 };
	}

	namespace CGrenades {
		inline constexpr std::ptrdiff_t pGrenadeList{ 0x18 };
	}
};