#pragma once
#include "DMA/DMA.h"
#include "json.hpp"

class ResponseData
{
public:
	static void Initialize(DMA_Connection* Conn);
	static void OnDMAFrame(DMA_Connection* Conn);
	static inline nlohmann::json LatestJson{};

private:
	using JsonBuff = std::array<char, 500000>;
	static inline uintptr_t m_JsonDataAddress{ 0x0 };
	static inline JsonBuff JsonBuffer{};

private:
	static bool UpdateJsonData(DMA_Connection* Conn);
	static bool ReadJsonBuffer(DMA_Connection* Conn);
	static nlohmann::json ParseBufferToJson();
	static std::string TrimJsonBuffer(JsonBuff& Buffer);
};