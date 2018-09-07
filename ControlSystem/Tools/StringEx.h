#ifndef STRINGEX_H
#define STRINGEX_H

#include <string>
#include <vector>

namespace R2D2 {

class StringEx
{
public:
	static bool startsWith(std::string src, std::string prefix);
	static bool endsWith(std::string src, std::string suffix);
	static void split(std::string src, char deliminator, std::vector<std::string>& list);
};

} // namespace R2D2

#endif // STRINGEX_H
