#pragma once
#include <cstddef>
#include "Vector.h"
// Created with ReClass.NET 1.2 by KN4CK3R

class CLocalGameWorld
{
public:
	char pad_0000[192]; //0x0000
	class CMapNameBuff* pMapName; //0x00C0
	char pad_00C8[208]; //0x00C8
	class N000001DF* pRegisteredPlayers; //0x0198
	void* N000000F7; //0x01A0
	class N00000182* N000000F8; //0x01A8
	char pad_01B0[56]; //0x01B0
	class CPlayer* pMainPlayer; //0x01E8
	char pad_01F0[840]; //0x01F0
}; //Size: 0x0538

class CMapNameBuff
{
public:
	char pad_0000[20]; //0x0000
	wchar_t N000001BA[64]; //0x0014
}; //Size: 0x0094

class N00000182
{
public:
	char pad_0000[264]; //0x0000
}; //Size: 0x0108

class N000001DF
{
public:
	char pad_0000[16]; //0x0000
	class N00000235* N000001E2; //0x0010
	char pad_0018[240]; //0x0018
}; //Size: 0x0108

class N00000235
{
public:
	class N00000381* N00000236; //0x0000
	char pad_0008[2112]; //0x0008
}; //Size: 0x0848

class N00000381
{
public:
	char pad_0000[3080]; //0x0000
}; //Size: 0x0C08

class CPlayer
{
public:
	char pad_0000[96]; //0x0000
	class CMovementContext* pMovementContext; //0x0060
	char pad_0068[296]; //0x0068
	class CPlayerBody* pPlayerBody; //0x0190
	char pad_0198[1744]; //0x0198
	class N000006D3* Location; //0x0868
	char pad_0870[112]; //0x0870
	class CProfile* pProfile; //0x08E0
	char pad_08E8[800]; //0x08E8
}; //Size: 0x0C08

class N000006D3
{
public:
	char pad_0000[72]; //0x0000
}; //Size: 0x0048

class CMovementContext
{
public:
	char pad_0000[72]; //0x0000
	void* pOwningPlayer; //0x0048
	char pad_0050[120]; //0x0050
	float Yaw; //0x00C8
}; //Size: 0x00CC

class CPlayerBody
{
public:
	char pad_0000[48]; //0x0000
	class CSkeleton* pSkeleton; //0x0030
	char pad_0038[976]; //0x0038
}; //Size: 0x0408

class CProfile
{
public:
	char pad_0000[16]; //0x0000
	void* pName; //0x0010
	void* pID; //0x0018
	char pad_0020[1000]; //0x0020
}; //Size: 0x0408

class CSkeleton
{
public:
	char pad_0000[48]; //0x0000
	class CValues* pValues; //0x0030
	char pad_0038[976]; //0x0038
}; //Size: 0x0408

class CValues
{
public:
	char pad_0000[16]; //0x0000
	class CArr1* pArr1; //0x0010
	char pad_0018[240]; //0x0018
}; //Size: 0x0108

class CArr1
{
public:
	char pad_0000[32]; //0x0000
	class CBoneTransforms* pBoneTransforms; //0x0020
	char pad_0028[224]; //0x0028
}; //Size: 0x0108

class CBoneTransforms
{
public:
	char pad_0000[16]; //0x0000
	class CUnityTransform* pBaseTransform; //0x0010
	char pad_0018[240]; //0x0018
}; //Size: 0x0108

class CTransformHierarchy
{
public:
	char pad_0000[8]; //0x0000
	uint64_t VerticiesOffset; //0x0008
	char pad_0010[144]; //0x0010
	uint64_t IndicesOffset; //0x00A0
	char pad_00A8[8]; //0x00A8
	Vector3 Position; //0x00B0
	char pad_00BC[92]; //0x00BC
}; //Size: 0x0118