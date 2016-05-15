#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

namespace R2D2 {

class Process
{
public:
	static int execute(std::string cmd);
	static bool isRunning(std::string);
	static pid_t getPID(std::string proc);
};

} // namespace R2D2

#endif // PROCESS_H
