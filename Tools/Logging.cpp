#include "Logging.h"

#include <iostream>
#include <iomanip>

using namespace R2D2;

void Logging::log(const std::string fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	std::cout << formatString(fmt, args).c_str() << std::endl;
	va_end(args);
}

void Logging::log(LogType type, const std::string fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	log(type, "", fmt, args);
	va_end(args);
}

void Logging::log(LogType type, const std::string source, const std::string fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	log(type, source, fmt, args);
	va_end(args);
}

void Logging::newLine()
{
	std::cout << std::endl;
}

void Logging::clearLine()
{
	std::cout << "\33[2K";
	std::cout << "\r";
}

void Logging::log(LogType type, const std::string source, const std::string fmt, va_list args)
{

#ifdef NO_DEBUG
	if (type == LOG_DEBUG) return;
#endif

	std::string str = formatString(fmt, args);

	switch (type)
	{
		case LOG_DEBUG:
			std::cout << "\033[2;37m" << "[D] ";
			break;

		case LOG_INFO:
			std::cout << "\033[0;32m" << "[I] ";
			break;

		case LOG_WARN:
			std::cout << "\033[1;33m" << "[W] ";
			break;

		case LOG_ERROR:
			std::cout << "\033[1;31m" << "[E] ";
			break;
	}

	std::string sourceTag;
	if (!source.empty())
	{
		sourceTag.append("[");
		sourceTag.append(source);
		sourceTag.append("]");
	}
	std::cout << std::left << std::setw(15) << sourceTag.c_str() << " ";
	std::cout << str.c_str() << "\033[0m" << std::endl;
}

std::string Logging::formatString(const std::string fmt, va_list args)
{
	int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
	std::string str;

	while (1)
	{     // Maximum two passes on a POSIX system...
		str.resize(size);
		int n = vsnprintf((char *)str.data(), size, fmt.c_str(), args);

		if (n > -1 && n < size) // Everything worked
		{
			str.resize(n);
			break;
		}

		if (n > -1)  // Needed size returned
			size = n + 1;   // For null char
		else
			size *= 2;      // Guess at a larger size (OS specific)
	}

	return str;
}
