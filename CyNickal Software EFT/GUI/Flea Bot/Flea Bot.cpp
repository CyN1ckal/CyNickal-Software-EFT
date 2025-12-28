#include "pch.h"
#include "Flea Bot.h"
#include "Game/Response Data/Response Data.h"
#include "Makcu/MyMakcu.h"
#include "Database/Database.h"

using namespace std::chrono_literals;

void ImGuiChronoEdit(const char* label, std::chrono::milliseconds& value)
{
	int TotalMs = static_cast<int>(value.count());
	ImGui::SetNextItemWidth(125.0f);
	if (ImGui::InputInt(label, &TotalMs))
	{
		if (TotalMs < 0)
			TotalMs = 0;
		value = std::chrono::milliseconds(TotalMs);
	}
}

void FleaBot::Render()
{
	if (m_PriceList.empty())
	{
		LoadPriceList();
		ConstructPriceList();
	}

	if (pInputThread && !bMasterToggle && bInputThreadDone)
	{
		std::println("[{0:%T}] [Flea Bot] Stopping input thread.", std::chrono::system_clock::now());
		pInputThread->join();
		pInputThread = nullptr;
	};

	if (!pInputThread && bMasterToggle)
	{
		bInputThreadDone = false;
		pInputThread = std::make_unique<std::thread>(&FleaBot::InputThread);
	}

	ImGui::Begin("Flea Bot");
	auto ContentRegion = ImGui::GetContentRegionAvail();
	if (ImGui::BeginChild("ChildL", { ContentRegion.x * 0.35f, ContentRegion.y }))
	{
		ImGui::Checkbox("Enable Flea Bot", &bMasterToggle);

		if (ImGui::BeginTabBar("MyTabBar"))
		{
			if (ImGui::BeginTabItem("Cycle"))
			{
				ImGui::Checkbox("Enable##Cycle", &bCycleBuy);
				ImGuiChronoEdit("Cycle Delay", CycleDelay);
				ImGuiChronoEdit("Super Cycley", SuperCycleDelay);
				ImGuiChronoEdit("Timeout", TimeoutDuration); 
				ImGui::SetNextItemWidth(125.0f);
				ImGui::InputScalar("# Items", ImGuiDataType_U32, &m_ItemCount);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Single"))
			{
				ImGui::Checkbox("Enable##Single", &bLimitBuy);
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	if (ImGui::BeginChild("ChildR", { ContentRegion.x * 0.65f, ContentRegion.y }))
	{
		ItemFilter.Draw("Filter Items");

		if (ImGui::Button("Save Price List"))
			SavePriceList();

		ImGui::SameLine();

		if (ImGui::Button("Reload Price List"))
		{
			m_PriceList.clear();
			LoadPriceList();
			ConstructPriceList();
		}

		if (ImGui::BeginTable("Price Limits", 3))
		{
			ImGui::TableSetupColumn("Item ID");
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Max Price");
			ImGui::TableHeadersRow();

			for (auto& [ItemID, PriceListEntry] : m_PriceList)
			{
				if (ItemFilter.IsActive() && !ItemFilter.PassFilter(PriceListEntry.ItemName.c_str()))
					continue;

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted(ItemID.c_str());
				ImGui::TableSetColumnIndex(1);
				ImGui::TextUnformatted(PriceListEntry.ItemName.c_str());
				ImGui::TableSetColumnIndex(2);
				ImGui::InputScalar(("##MaxPriceInput" + ItemID).c_str(), ImGuiDataType_U32, &PriceListEntry.MaxPrice);
			}

			ImGui::EndTable();
		}
	}
	ImGui::EndChild();

	ImGui::End();
}

void FleaBot::OnNewResponse()
{
	auto& Json = ResponseData::LatestJson;

	auto ResponseType = IdentifyResponse(Json);

	if (ResponseType == EResponseType::Offer)
	{
		bHasNewOfferData = true;
		std::scoped_lock Lock(LastOfferMut);
		LatestOfferJson = Json;
	}
}

void FleaBot::InputThread()
{
	std::println("[{0:%T}] [Flea Bot] Input thread started.", std::chrono::system_clock::now());

	while (bMasterToggle)
	{
		if (bLimitBuy)
			LimitBuyOneLogic({ 1840,120 });

		if (bCycleBuy)
			CycleBuy({ 1840,120 });

		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}

	std::println("[{0:%T}] [Flea Bot] Input thread joining", std::chrono::system_clock::now());
	bInputThreadDone = true;
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

namespace SimpleInput
{
	namespace Positions
	{
		const CMousePos BrowseButton{ 105,85 };
		const CMousePos WishlistButton{ 245,85 };
		const CMousePos RefreshButton{ 1840,120 };
		const CMousePos PurchaseButton{ 1775,180 };
		const CMousePos AllButton{ 1150,490 };
		const CMousePos YesButton{ 865,605 };
		const CMousePos OkButton{ 960,580 };
		const CMousePos MainMenuButton{ 80,1065 };
		const CMousePos FleaMarketButton{ 1230,1065 };
		const CMousePos BuildingMaterialFirstEntry{ 500,265 };
		const CMousePos FleaMarketTopCategory{ 500,165 };
	}
	namespace Sizes
	{
		const CMousePos FleaMarketCategory{ 0,34 };
		const CMousePos WishlistItem{ 0,32 };
		const CMousePos FleaMarketItemEntry{ 0,25 };
	}

	void Click()
	{
		MyMakcu::m_Device.mouseDown(makcu::MouseButton::LEFT);
		std::this_thread::sleep_for(10ms);
		MyMakcu::m_Device.mouseUp(makcu::MouseButton::LEFT);
	}
	void ClickThenWait(std::chrono::milliseconds delay)
	{
		Click();
		std::this_thread::sleep_for(delay);
	}
	CMousePos MoveTo(CMousePos StartingPos, CMousePos DesiredPos)
	{
		auto Delta = DesiredPos - StartingPos;
		MyMakcu::m_Device.mouseMove(Delta.x, Delta.y);
		return DesiredPos;
	}
	CMousePos MoveToThenClick(CMousePos StartingPos, CMousePos DesiredPos, std::chrono::milliseconds delay)
	{
		auto CurPos = MoveTo(StartingPos, DesiredPos);
		std::this_thread::sleep_for(delay);
		Click();
		std::this_thread::sleep_for(10ms);
		return CurPos;
	}
	CMousePos MoveToThenWait(CMousePos StartingPos, CMousePos DesiredPos, std::chrono::milliseconds delay)
	{
		auto CurPos = MoveTo(StartingPos, DesiredPos);
		std::this_thread::sleep_for(delay);
		return CurPos;
	}
	CMousePos MoveToRefreshButton(CMousePos StartingPos)
	{
		return MoveTo(StartingPos, Positions::RefreshButton);
	}
}
const auto ShortDelay = 50ms;
const auto MediumDelay = std::chrono::milliseconds(500);
const auto LongDelay = std::chrono::seconds(2);
void FleaBot::BuyFirstItemStack(CMousePos StartingPos)
{
	auto CurPos = SimpleInput::MoveToThenClick(StartingPos, SimpleInput::Positions::PurchaseButton, ShortDelay);
	CurPos = SimpleInput::MoveToThenClick(CurPos, SimpleInput::Positions::AllButton, ShortDelay);
	CurPos = SimpleInput::MoveToThenClick(CurPos, SimpleInput::Positions::YesButton, ShortDelay);

	/* Wait for order to process */
	std::this_thread::sleep_for(LongDelay);
	CurPos = SimpleInput::MoveToThenClick(CurPos, SimpleInput::Positions::OkButton, ShortDelay);

	SimpleInput::MoveTo(CurPos, StartingPos);
}

void FleaBot::ReturnToMainMenuAndWait(CMousePos StartingPos, std::chrono::seconds Delay)
{
	std::println("[{0:%T}] [Flea Bot] Waiting {1:d} seconds on main menu", std::chrono::system_clock::now(), Delay.count());
	auto CurPos = SimpleInput::MoveToThenClick(StartingPos, SimpleInput::Positions::MainMenuButton, 100ms);
	std::this_thread::sleep_for(Delay);

	std::println("[{0:%T}] [Flea Bot] Resuming...", std::chrono::system_clock::now());
	CurPos = SimpleInput::MoveToThenClick(CurPos, SimpleInput::Positions::FleaMarketButton, 100ms);

	/* wait for UI to load */
	std::this_thread::sleep_for(1s);
	CurPos = SimpleInput::MoveToThenClick(CurPos, SimpleInput::Positions::WishlistButton, 100ms);

	SimpleInput::MoveTo(CurPos, StartingPos);
}

void FleaBot::CycleBuy(CMousePos StartingPos)
{
	bHasNewOfferData = false;

	auto CurPos = SimpleInput::MoveToThenClick(StartingPos, SimpleInput::Positions::FleaMarketTopCategory, CycleDelay);

	CurPos = SimpleInput::MoveToThenClick(CurPos, SimpleInput::Positions::RefreshButton, CycleDelay);

	size_t Iterations{ 0 };
	size_t FailedAttempts{ 0 };
	do
	{
		if (Iterations == 0)
			CurPos = SimpleInput::MoveTo(CurPos, SimpleInput::Positions::FleaMarketTopCategory);
		else
		{
			CurPos = SimpleInput::MoveToThenClick(CurPos, CurPos + SimpleInput::Sizes::WishlistItem, CycleDelay);
			bHasNewOfferData = false;
		}

		auto OfferJson = AwaitNewOfferData(TimeoutDuration);

		if (OfferJson.is_null())
		{
			FailedAttempts++;
			std::println("[{0:%T}] [Flea Bot] No new data received for item {1:d}; failed attempts: {2:d}", std::chrono::system_clock::now(), Iterations, FailedAttempts);

			if (FailedAttempts >= 3)
			{
				std::println("[{0:%T}] [Flea Bot] Too many failed attempts; stopping flea bot.", std::chrono::system_clock::now());
				ReturnToMainMenuAndWait(CurPos, 30s);
				SimpleInput::MoveTo(CurPos, StartingPos);
				return;
			}
		}
		else
		{
			auto& FirstOffer = OfferJson["data"]["offers"][0];
			if (DoesOfferPassPriceListCheck(FirstOffer))
			{
				/* Waiting for response to populate UI */
				std::this_thread::sleep_for(20ms);
				BuyFirstItemStack(CurPos);
			}
		}

		Iterations++;

	} while (Iterations < m_ItemCount && bMasterToggle);

	SimpleInput::MoveTo(CurPos, StartingPos);
}

nlohmann::json FleaBot::AwaitNewOfferData(std::chrono::milliseconds Timeout)
{
	const auto StartTime = std::chrono::steady_clock::now();
	bool bFailed{ false };
	auto Return = nlohmann::json();

	while (bHasNewOfferData == false && bFailed == false)
	{
		std::chrono::milliseconds Elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - StartTime);

		if (Elapsed >= Timeout)
		{
			bFailed = true;
			std::println("[{0:%T}] [Flea Bot] AwaitNewOfferData timed out after {1:d} ms.", std::chrono::system_clock::now(), Elapsed.count());
		}
	}

	if (bFailed == false)
	{
		std::scoped_lock Lock(LastOfferMut);
		Return = LatestOfferJson;
	}

	return Return;
}

void FleaBot::ConstructPriceList()
{
	m_PriceList.clear();
	for (const auto& [ItemID, MaxPrice] : m_DefaultPrices)
	{
		CPriceListEntry NewEntry;
		NewEntry.MaxPrice = MaxPrice;
		NewEntry.ItemName = TarkovItemData::GetShortNameOfItem(ItemID);
		m_PriceList[ItemID] = NewEntry;
	}
}

void FleaBot::LoadPriceList()
{
	std::ifstream PriceListFile("FleaBot_PriceList.json", std::ios::in);
	if (PriceListFile.is_open())
	{
		nlohmann::json JsonData;
		PriceListFile >> JsonData;
		PriceListFile.close();
		for (auto& Entry : JsonData)
		{
			auto ItemID = Entry[0].get<std::string>();
			auto MaxPrice = Entry[1].get<uint32_t>();
			m_DefaultPrices[ItemID] = MaxPrice;
		}
	}
}

void FleaBot::SavePriceList()
{
	std::ofstream PriceListFile("FleaBot_PriceList.json", std::ios::out | std::ios::trunc);

	if (PriceListFile.is_open())
	{
		auto JsonData = PriceListToJson();
		PriceListFile << JsonData.dump(4);
		PriceListFile.close();
	}
}

bool FleaBot::DoesOfferPassPriceListCheck(const nlohmann::json& OfferJson)
{
	try
	{
		auto Price = OfferJson["requirements"][0]["count"].get<int>();
		auto ItemId = OfferJson["items"][0]["_tpl"].get<std::string>();
		auto CurrencyType = OfferJson["requirements"][0]["_tpl"].get<std::string>();
		auto Quantity = OfferJson["quantity"].get<int>();

		if (Quantity < 1)
			return false;

		if (CurrencyType != RoubleBSGID)
			return false;

		auto It = m_PriceList.find(ItemId);
		if (It == m_PriceList.end())
			return false;

		if (Price > It->second.MaxPrice)
			return false;

		std::println("[{0:%T}] [Flea Bot] Buying {1:d} {2:s} for {3:d} each", std::chrono::system_clock::now(), Quantity, It->second.ItemName, Price);

		return true;
	}
	catch (const nlohmann::json::exception& e)
	{
		std::println("[{0:%T}] [Flea Bot] JSON exception in DoesOfferPassPriceListCheck: {1:s}", std::chrono::system_clock::now(), e.what());
		return false;
	}
}

nlohmann::json FleaBot::PriceListToJson()
{
	auto Return = nlohmann::json();

	for (auto& [ItemID, PriceEntry] : m_PriceList)
		Return.push_back({ ItemID,PriceEntry.MaxPrice });

	return Return;
}

void FleaBot::LimitBuyOneLogic(CMousePos StartingPos)
{
	std::scoped_lock Lock(LastOfferMut);

	if (LatestOfferJson.contains("data") == false || LatestOfferJson["data"].contains("offers") == false || LatestOfferJson["data"]["offers"].empty())
		return;

	auto& BestOffer = LatestOfferJson["data"]["offers"][0];
	auto OfferId = BestOffer["_id"].get<std::string>();
	static std::string LastOfferId{ "" };
	if (OfferId != LastOfferId && DoesOfferPassPriceListCheck(LatestOfferJson["data"]["offers"][0]))
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