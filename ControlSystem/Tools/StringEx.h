#ifndef STRINGEX_H
#define STRINGEX_H

#include <string>

namespace R2D2 {

class StringEx
{
public:
	static bool startsWith(std::string src, std::string prefix);
	static bool endsWith(std::string src, std::string suffix);
};

} // namespace R2D2

#endif // STRINGEX_H
