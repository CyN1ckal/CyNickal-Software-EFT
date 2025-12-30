#include "pch.h"
#include "CFileDownload.hpp"
#include "Network/Callbacks/Callbacks.hpp"

CFileDownload::CFileDownload(const std::string& URL)
{
	std::println("[CFileDownload] Downloading from URL: {}", URL);

	auto curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callbacks::WriteToString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m_Response);

	auto Res = curl_easy_perform(curl);

	if (Res != CURLE_OK)
		std::println("[CFileDownload] curl_easy_perform() failed: {}", curl_easy_strerror(Res));

	curl_easy_cleanup(curl);

	std::println("[CFileDownload] Download complete. Response size: {} bytes", m_Response.size());
}

const std::string& CFileDownload::GetResponse() const
{
	return m_Response;
}