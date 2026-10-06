/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "DMA/CDMAConnection/CDMAConnection.h"
#include "DMA/CModule/CModule.h"

namespace ConstStrings {
	const std::string Game = "EscapeFromTarkov.exe";
	const std::string Unity = "UnityPlayer.dll";
	const std::string GameAssembly = "GameAssembly.dll";
}

class CProcess
{
private:
	DWORD m_PID{ 0 };
	std::optional<std::unordered_map<std::string, CModule>> m_Modules{ std::nullopt };

public:
	bool GetProcessInfo(CDMAConnection* Conn);
	const uintptr_t GetBaseAddress() const;
	const uintptr_t GetUnityAddress() const;
	const uintptr_t GetAssemblyBase() const;
	const DWORD GetPID() const;
	const uintptr_t GetModuleAddress(const std::string& ModuleName);
	CModule& GetModule(const std::string ModuleName);

private:
	bool PopulateModules(CDMAConnection* Conn);

public:
	template<typename T> inline T ReadMem(CDMAConnection* Conn, uintptr_t Address) const
	{
		VMMDLL_SCATTER_HANDLE vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), m_PID, VMMDLL_FLAG_NOCACHE);
		DWORD BytesRead{ 0 };
		T Buffer{};

		VMMDLL_Scatter_PrepareEx(vmsh, Address, sizeof(T), reinterpret_cast<BYTE*>(&Buffer), &BytesRead);

		VMMDLL_Scatter_Execute(vmsh);

		VMMDLL_Scatter_CloseHandle(vmsh);

		if (BytesRead != sizeof(T))
			std::println("Incomplete read: {}/{}", BytesRead, sizeof(T));

		return Buffer;
	}
	template<typename T> inline std::optional<T> OptionalReadMem(CDMAConnection* Conn, uintptr_t Address) const
	{
		VMMDLL_SCATTER_HANDLE vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), m_PID, VMMDLL_FLAG_NOCACHE);
		DWORD BytesRead{ 0 };
		T Buffer{};

		VMMDLL_Scatter_PrepareEx(vmsh, Address, sizeof(T), reinterpret_cast<BYTE*>(&Buffer), &BytesRead);

		VMMDLL_Scatter_Execute(vmsh);

		VMMDLL_Scatter_CloseHandle(vmsh);

		if (BytesRead != sizeof(T))
			return std::nullopt;

		return Buffer;
	}
	template <typename T> inline void WriteMem(CDMAConnection* Conn, uintptr_t Address, T& Value) const
	{
		VMMDLL_SCATTER_HANDLE vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), GetPID(), VMMDLL_FLAG_NOCACHE);
		VMMDLL_Scatter_PrepareWrite(vmsh, Address, reinterpret_cast<BYTE*>(&Value), sizeof(T));
		VMMDLL_Scatter_Execute(vmsh);
		VMMDLL_Scatter_CloseHandle(vmsh);
	}
	template<typename T> inline std::vector<T> ReadVec(CDMAConnection* Conn, uintptr_t Address, size_t Num) const {
		VMMDLL_SCATTER_HANDLE vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), m_PID, VMMDLL_FLAG_NOCACHE);
		DWORD BytesRead{ 0 };

		std::vector<T> Buffer(Num);

		const auto ReadSize = sizeof(T) * Num;

		VMMDLL_Scatter_PrepareEx(vmsh, Address, ReadSize, reinterpret_cast<BYTE*>(Buffer.data()), &BytesRead);

		VMMDLL_Scatter_Execute(vmsh);

		VMMDLL_Scatter_CloseHandle(vmsh);

		if (BytesRead != ReadSize) {
			std::println("[CProcess] Incomplete vec read: {}/{}", BytesRead, ReadSize);
			Buffer.resize(BytesRead / sizeof(T));
		}

		return Buffer;
	}
	inline uintptr_t ReadChain(CDMAConnection* Conn, uintptr_t Base, std::vector<std::ptrdiff_t> Offsets) const
	{
		uintptr_t PreviousAddress = Base;
		for (auto& Offset : Offsets)
			PreviousAddress = ReadMem<uintptr_t>(Conn, PreviousAddress + Offset);

		return PreviousAddress;
	}
	inline bool ReadBuffer(CDMAConnection* Conn, uintptr_t Address, BYTE* Buffer, size_t Size) const
	{
		VMMDLL_SCATTER_HANDLE vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), m_PID, VMMDLL_FLAG_NOCACHE);
		DWORD BytesRead{ 0 };
		VMMDLL_Scatter_PrepareEx(vmsh, Address, static_cast<DWORD>(Size), Buffer, &BytesRead);
		VMMDLL_Scatter_Execute(vmsh);
		VMMDLL_Scatter_CloseHandle(vmsh);

		return BytesRead == Size;
	}

	template <typename T>
	inline std::basic_string<T> ReadUnityString(CDMAConnection* Conn, uintptr_t StringObjectAddress) const
	{
		if (StringObjectAddress == 0)
			return std::basic_string<T>();

		const int StringLength = ReadMem<int>(Conn, StringObjectAddress + 0x10);

		if (StringLength <= 0 || StringLength > 1024)
			return std::basic_string<T>();

		std::basic_string<T> Result;

		Result.resize(StringLength);

		ReadBuffer(Conn, StringObjectAddress + 0x14, reinterpret_cast<BYTE*>(Result.data()), StringLength * sizeof(T));

		return Result;
	}

	template <typename T>
	inline std::basic_string<T> ReadUnityStringPtr(CDMAConnection* Conn, uintptr_t Address) const
	{
		const uintptr_t StringObjectAddress = ReadMem<uintptr_t>(Conn, Address);

		return ReadUnityString<T>(Conn, StringObjectAddress);
	}
};