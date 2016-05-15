#include "Process.h"

#include <dirent.h>		// for DIR
#include <stdlib.h>		// for atol()
#include <stdio.h>		// for snprintf(), fopen(), fclose(), fscanf()
#include <string.h>		// for strcmp()
#include <fstream>		// for ifstream
#include <unistd.h>		// for exec
#include "Logging.h"

using namespace R2D2;

int Process::execute(std::string cmd)
{
	Logging::log(LOG_DEBUG, "PROC", "Exec: %s", cmd.c_str());
	return system(cmd.c_str());
}

bool Process::isRunning(std::string proc)
{
	return (getPID(proc) > 0);
}

pid_t Process::getPID(std::string proc)
{
	int pid = -1;

	// Open the /proc directory
	DIR *dp = opendir("/proc");
	if (dp != NULL)
	{
		// Enumerate all entries in directory until process found
		struct dirent *dirp;
		while (pid < 0 && (dirp = readdir(dp)))
		{
			// Skip non-numeric entries
			int id = atoi(dirp->d_name);
			if (id > 0)
			{
				// Read contents of virtual /proc/{pid}/cmdline file
				std::string cmdPath = std::string("/proc/") + dirp->d_name + "/cmdline";
				std::ifstream cmdFile(cmdPath.c_str());
				std::string cmdLine;
				getline(cmdFile, cmdLine);
				if (!cmdLine.empty())
				{
					// Keep first cmdline item which contains the program path
					size_t pos = cmdLine.find('\0');
					if (pos != std::string::npos)
						cmdLine = cmdLine.substr(0, pos);
					// Keep program name only, removing the path
					pos = cmdLine.rfind('/');
					if (pos != std::string::npos)
						cmdLine = cmdLine.substr(pos + 1);
					// Compare against requested process name
					if (proc == cmdLine)
						pid = id;
				}
			}
		}
	}

	closedir(dp);

	return pid;
}
