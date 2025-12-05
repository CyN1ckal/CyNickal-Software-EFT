#pragma once
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Classes/Vector.h"

enum class EPlayerType
{
	eMainPlayer,
	eObservedPlayer
};

enum class EPlayerSide : uint32_t
{
	USEC = 0,
	BEAR = 2,
	SCAV = 4,
	UNKNOWN = std::numeric_limits<uint32_t>::max()
};

enum class ESpawnType : uint32_t
{
	marksman = 0,
	assault = 1,
	bossTest = 2,
	Reshala = 3,
	followerTest = 4,
	followerBully = 5,
	Killa = 6,
	Shturman = 7,
	followerKojaniy = 8,
	pmcBot = 9,
	cursedAssault = 10,
	Gluhar = 11,
	followerGluharAssault = 12,
	followerGluharSecurity = 13,
	followerGluharScout = 14,
	followerGluharSnipe = 15,
	followerSanitar = 16,
	Sanitar = 17,
	test = 18,
	assaultGroup = 19,
	sectantWarrior = 20,
	sectantPriest = 21,
	Tagilla = 22,
	followerTagilla = 23,
	exUsec = 24,
	gifter = 25,
	Knight = 26,
	BigPipe = 27,
	BirdEye = 28,
	Zryachiy = 29,
	followerZryachiy = 30,
	bossBoar = 32,
	followerBoar = 33,
	arenaFighter = 34,
	arenaFighterEvent = 35,
	bossBoarSniper = 36,
	crazyAssaultEvent = 37,
	peacefullZryachiyEvent = 38,
	sectactPriestEvent = 39,
	ravangeZryachiyEvent = 40,
	followerBoarClose1 = 41,
	followerBoarClose2 = 42,
	Kolontay = 43,
	followerKolontayAssault = 44,
	followerKolontaySecurity = 45,
	shooterBTR = 46,
	Partisan = 47,
	spiritWinter = 48,
	spiritSpring = 49,
	peacemaker = 50,
	pmcBEAR = 51,
	pmcUSEC = 52,
	skier = 53,
	sectantPredvestnik = 57,
	sectantPrizrak = 58,
	sectantOni = 59,
	infectedAssault = 60,
	infectedPmc = 61,
	infectedCivil = 62,
	infectedLaborant = 63,
	infectedTagilla = 64,
	bossTagillaAgro = 65,
	bossKillaAgro = 66,
	tagillaHelperAgro = 67,
	blackDivision = 68,
	vsRF = 69,
	vsRFSniper = 70,
	assaultTutorial = 71,
	sentry = 72,
	vsRFFight = 73,
	civilian = 74,
	UNKNOWN = std::numeric_limits<uint32_t>::max()
};

class CBaseEFTPlayer : public CBaseEntity
{
public:
	Vector3 m_RootPosition{};
	float m_Yaw{ 0.0f };
	EPlayerSide m_Side{ EPlayerSide::UNKNOWN };
	ESpawnType m_SpawnType{ ESpawnType::UNKNOWN };
	std::byte m_AiByte{ 0 };

private:
	uintptr_t m_PlayerBodyAddress{ 0 };
	uintptr_t m_SkeletonRootAddress{ 0 };
	uintptr_t m_SkeletonValuesAddress{ 0 };
	uintptr_t m_ValuesArrayAddress{ 0 };
	uintptr_t m_Arr1Address{ 0 };
	uintptr_t m_BoneTransformsAddress{ 0 };
	uintptr_t m_BasePositionTransformAddress{ 0 };
	uintptr_t m_TransformHierarchyAddress{ 0 };
	uintptr_t m_AIDataAddress{ 0 };
	uintptr_t m_BotOwnerAddress{ 0 };
	uintptr_t m_SpawnProfileDataAddress{ 0 };

public:
	CBaseEFTPlayer(uintptr_t EntityAddress) : CBaseEntity(EntityAddress) {}
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh, EPlayerType playerType);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickFinalize();

public:
	const bool IsAi() const;
	const bool IsPMC() const;
	const bool IsPlayerScav() const;
	const std::string& GetBaseName() const;
	const ImColor GetSideColor() const;
	const bool IsBoss() const;

private:
	const std::string& GetBossName() const;
};