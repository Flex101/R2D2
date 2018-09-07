#include "StringEx.h"
#include "Logging.h"

#include <sstream>		// for std::istringstream
#include <iterator>		// for std::istream_iterator

using namespace R2D2;

bool StringEx::startsWith(std::string src, std::string prefix)
{
	return src.substr(0, prefix.length()) == prefix;
}

bool StringEx::endsWith(std::string src, std::string suffix)
{
	return src.substr(src.length() - suffix.length()) == suffix;
}

void StringEx::split(std::string src, char deliminator, std::vector<std::string>& list)
{
	std::stringstream ss(src);
	std::string token;
	while (std::getline(ss, token, deliminator))
	{
		list.push_back(token);
	}
}
