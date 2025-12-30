#include "pch.h"

#include "Callbacks.hpp"

size_t Callbacks::WriteToString(char* contents, size_t size, size_t nmemb, std::string* userp)
{
	size_t totalSize = size * nmemb;
	userp->append(contents, totalSize);
	return totalSize;
}
