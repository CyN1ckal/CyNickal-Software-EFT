#pragma once

class CFileDownload
{
public:
	CFileDownload(const std::string& URL);
	const std::string& GetResponse() const;

private:
	std::string m_Response{};
};