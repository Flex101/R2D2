#include "StringEx.h"
#include "Logging.h"

using namespace R2D2;

bool StringEx::startsWith(std::string src, std::string prefix)
{
	return src.substr(prefix.length()) == prefix;
}

bool StringEx::endsWith(std::string src, std::string suffix)
{
	return src.substr(src.length() - suffix.length()) == suffix;
}
