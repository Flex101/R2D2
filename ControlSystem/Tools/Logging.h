#ifndef LOGGING_H
#define LOGGING_H

#include <stdarg.h>
#include <string>

namespace R2D2 {

enum LogType
{
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR
};

class Logging
{

public:
	static void log(const std::string message, ...);
	static void log(LogType type, const std::string message, ...);
	static void log(LogType type, const std::string source, const std::string message, ...);

	static void newLine();
	static void clearLine();

protected:
	static void log(LogType type, const std::string source, const std::string message, va_list args);
	static std::string formatString(const std::string message, va_list args);
};

} // namespace R2D2

#endif // LOGGING_H
