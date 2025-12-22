#include "pch.h"
#include "Flea Bot.h"
#include "Game/Response Data/Response Data.h"
#include "Makcu/MyMakcu.h"

using namespace std::chrono_literals;

void FleaBot::Render()
{
	if (pThread && !bMasterToggle)
	{
		std::println("[Flea Bot] Stopping input thread...");
		pThread->join();
		pThread = nullptr;
	};

	if (!pThread && bMasterToggle)
	{
		std::println("[Flea Bot] Starting input thread...");
		pThread = std::make_unique<std::thread>(&FleaBot::InputThread);
	}

	ImGui::Begin("Flea Bot");
	ImGui::Checkbox("Enable Flea Bot", &bMasterToggle);

	ImGui::Checkbox("Cycle Building Materials", &bCycleBuildingMaterials);
	ImGui::Checkbox("Limit Buying", &bLimitBuy);
	ImGui::End();
}

void FleaBot::OnNewResponse()
{
	auto& Json = ResponseData::LatestJson;

	auto ResponseType = IdentifyResponse(Json);

	if (ResponseType == EResponseType::INVALID)
	{
		std::println("[FleaBot] Invalid response received.");
	}
	else if (ResponseType == EResponseType::Offer)
	{
		std::println("[FleaBot] Offer response received.");
		bHasNewData = true;
		std::scoped_lock Lock(JsonMutex);
		LatestOfferJson = Json;
	}
}

void FleaBot::InputThread()
{
	std::println("[Flea Bot] Input thread started.");

	while (bMasterToggle)
	{
		if (bLimitBuy)
			LimitBuyOneLogic({ 1840,120 });

		if (bCycleBuildingMaterials)
			CycleBuildingMaterials({ 1840,120 });

		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}

	std::println("[Flea Bot] Input thread joining");
}

EResponseType FleaBot::IdentifyResponse(const nlohmann::json& ResponseJson)
{
	EResponseType Return = EResponseType::INVALID;

	if (ResponseJson.contains("data") == false) return Return;

	if (ResponseJson["data"].contains("offers"))
		Return = EResponseType::Offer;

	return Return;
}

void FleaBot::PrintOffers(const nlohmann::json& ResponseJson)
{
	const auto& Offers = ResponseJson["data"]["offers"];

	for (auto& Offer : Offers)
	{
		auto Username = Offer["user"]["nickname"].get<std::string>();
		auto ItemID = Offer["items"][0]["_tpl"].get<std::string>();
		auto Quantity = Offer["quantity"].get<int>();
		auto Price = Offer["requirements"][0]["count"].get<int>();
		std::println("   {} offering {} {} for {} each", Username.c_str(), ItemID, Quantity, Price);
	}
}

bool FleaBot::DoesOfferMeetCriteria(const nlohmann::json& OfferJson)
{
	auto Price = OfferJson["requirements"][0]["count"].get<int>();
	auto ItemId = OfferJson["items"][0]["_tpl"].get<std::string>();
	auto CurrencyType = OfferJson["requirements"][0]["_tpl"].get<std::string>();
	auto Quantity = OfferJson["quantity"].get<int>();

	if (Quantity < 1)
		return false;

	if (CurrencyType != RoubleBSGID)
		return false;

	uint32_t MaxPriceForItem = GetMaxPrice(ItemId);

	if (Price > MaxPriceForItem)
		return false;

	std::println("[Flea Bot] Found offer meeting criteria: Price = {}, Currency = {}, Quantity = {}", Price, CurrencyType, Quantity);

	return true;
}

void FleaBot::ClickInFiveSeconds()
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
	MyMakcu::m_Device.click(makcu::MouseButton::LEFT);
}

void FleaBot::HandleOffers(const nlohmann::json& OfferJson)
{
	auto FirstOffer = OfferJson[0];

	if (DoesOfferMeetCriteria(FirstOffer))
		bRequestedBuy = true;
}

const auto ShortDelay = 50ms;
const auto MediumDelay = std::chrono::milliseconds(500);
const auto LongDelay = std::chrono::seconds(2);
void ClickThenWait(std::chrono::milliseconds delay)
{
	MyMakcu::m_Device.mouseDown(makcu::MouseButton::LEFT);
	std::this_thread::sleep_for(10ms);
	MyMakcu::m_Device.mouseUp(makcu::MouseButton::LEFT);
	std::this_thread::sleep_for(delay);
}
CMousePos MoveThenWait(CMousePos PreviousPos, CMousePos Delta, std::chrono::milliseconds delay)
{
	auto FinalPos = PreviousPos + Delta;
	MyMakcu::m_Device.mouseMove(Delta.x, Delta.y);
	std::this_thread::sleep_for(delay);
	return FinalPos;
}
CMousePos MoveThenClick(CMousePos PreviousPos, CMousePos Delta, std::chrono::milliseconds delay)
{
	auto FinalPos = PreviousPos + Delta;

	MoveThenWait(PreviousPos, Delta, delay);

	ClickThenWait(15ms);

	return FinalPos;
}

void FleaBot::BuyFirstItemStack(CMousePos StartingPos)
{
	std::println("[Flea Bot] Buying top item...");

	CMousePos DesiredPos{ 1775, 180 };
	auto Delta = DesiredPos - StartingPos;

	auto CurPos = MoveThenClick(StartingPos, Delta, ShortDelay);
	CurPos = MoveThenClick(CurPos, { -625,310 }, ShortDelay);
	CurPos = MoveThenClick(CurPos, { -285,115 }, ShortDelay);

	/* Wait for order to process */
	std::this_thread::sleep_for(LongDelay);
	CurPos = MoveThenClick(CurPos, { 95, -25 }, ShortDelay);

	Delta = StartingPos - CurPos;
	MyMakcu::m_Device.mouseMove(Delta.x, Delta.y);
}

void FleaBot::CycleBuildingMaterials(CMousePos StartingPos)
{
	using namespace std::chrono_literals;

	const auto ConstructionListStart = CMousePos{ 559,240 };
	const auto Delta = ConstructionListStart - StartingPos;

	auto CurPos = MoveThenWait(StartingPos, Delta, 300ms);

	constexpr size_t ItemRows = 17;
	for (int i = 0; i < ItemRows && bMasterToggle; i++)
	{
		bHasNewData = false;
		CurPos = MoveThenClick(CurPos, { 0,25 }, 250ms);

		auto StartTime = std::chrono::steady_clock::now();
		while (!bHasNewData && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - StartTime).count() < 1000)
		{
			std::println("[Flea Bot] Waiting for new data...");
		}

		if (bHasNewData)
		{
			std::println("[Flea Bot] New data received during building materials cycle.");

			std::scoped_lock Lock(JsonMutex);
			HandleOffers(LatestOfferJson["data"]["offers"]);

			auto& FirstOffer = LatestOfferJson["data"]["offers"][0];
			if (DoesOfferMeetCriteria(FirstOffer))
			{
				/* Waiting for response to populate UI */
				std::this_thread::sleep_for(20ms);
				BuyFirstItemStack(CurPos);
			}
		}

		std::this_thread::sleep_for(15ms);
	}

	MoveThenClick(CurPos, StartingPos - CurPos, ShortDelay);
}

void FleaBot::LimitBuyOneLogic(CMousePos StartingPos)
{
	std::scoped_lock Lock(JsonMutex);

	if (LatestOfferJson.contains("data") == false || LatestOfferJson["data"].contains("offers") == false || LatestOfferJson["data"]["offers"].empty())
		return;

	auto& BestOffer = LatestOfferJson["data"]["offers"][0];
	auto OfferId = BestOffer["_id"].get<std::string>();
	static std::string LastOfferId{ "" };
	if (OfferId != LastOfferId && DoesOfferMeetCriteria(LatestOfferJson["data"]["offers"][0]))
	{
		BuyFirstItemStack(StartingPos);
		LastOfferId = OfferId;
	}

	static std::chrono::time_point<std::chrono::steady_clock> LastRefreshTime = std::chrono::steady_clock::now();
	auto CurrentTime = std::chrono::steady_clock::now();
	if (std::chrono::duration_cast<std::chrono::seconds>(CurrentTime - LastRefreshTime).count() > 4)
	{
		MyMakcu::m_Device.click(makcu::MouseButton::LEFT);
		LastRefreshTime = CurrentTime;
	}
}