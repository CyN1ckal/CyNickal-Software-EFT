#include "pch.h"
#include "Response Data.h"
#include "Game/EFT.h"
#include "Game/Offsets/Offsets.h"
#include "GUI/Flea Bot/Flea Bot.h"

void ResponseData::Initialize(DMA_Connection* Conn)
{
	auto& Proc = EFT::GetProcess();

	uintptr_t zlibObject = Proc.ReadMem<uintptr_t>(Conn, Proc.GetAssemblyBase() + Offsets::ZLibObject);
	m_JsonDataAddress = Proc.ReadChain(Conn, zlibObject, { 0xB8, 0x8, 0x28, 0x28 }) + 0x20;

	std::println("[ResponseData] JSON Data Address: 0x{:X}", m_JsonDataAddress);
}

void ResponseData::OnDMAFrame(DMA_Connection* Conn)
{
	if (!FleaBot::bMasterToggle)
		return;

	static uintptr_t PreviousData{ 0x0 };

	auto& Proc = EFT::GetProcess();

	uintptr_t CurrentData = Proc.ReadMem<uintptr_t>(Conn, m_JsonDataAddress + 0x38); // 0x38 is the end of the first offer ID

	if (CurrentData == PreviousData)
		return;

	if (!ResponseData::UpdateJsonData(Conn))
	{
		std::println("[Response Data] Failed to update JSON data.");
		return;
	}

	PreviousData = CurrentData;

	FleaBot::OnNewResponse();
}

bool ResponseData::UpdateJsonData(DMA_Connection* Conn)
{
	if (!ReadJsonBuffer(Conn))
	{
		std::println("[Response Data] Failed to read JSON buffer.");
		return false;
	}

	try
	{
		LatestJson = ParseBufferToJson();
	}
	catch (const nlohmann::json::parse_error& e)
	{
		std::println("[Response Data] JSON Parse Error: {}", e.what());
		return false;
	}

	return true;
}

bool ResponseData::ReadJsonBuffer(DMA_Connection* Conn)
{
	if(!m_JsonDataAddress)
		return false;

	JsonBuffer.fill(0);

	/* Read may be incomplete; but continue with whatever we have */
	EFT::GetProcess().ReadBuffer(Conn, m_JsonDataAddress, reinterpret_cast<BYTE*>(JsonBuffer.data()), JsonBuffer.size());

	return true;
}

nlohmann::json ResponseData::ParseBufferToJson()
{
	return nlohmann::json::parse(TrimJsonBuffer(JsonBuffer));
}

std::string ResponseData::TrimJsonBuffer(JsonBuff& Buffer)
{
	std::string ReturnString{};

	uint32_t BraceCount{ 0 };

	for (auto& Char : Buffer)
	{
		if (Char == '{')
			BraceCount++;
		else if (Char == '}')
			BraceCount--;

		if (BraceCount == 0)
		{
			ReturnString.assign(Buffer.data(), (&Char - Buffer.data()) + 1);
			break;
		}
	}

	return ReturnString;
}