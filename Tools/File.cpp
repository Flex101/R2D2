#include "File.h"
#include <sys/stat.h>		// for stat struct
#include <unistd.h>			// for cwd
#include <dirent.h>			// for DIR
#include <string.h>			// for strcmp
#include "StringEx.h"
#include "Logging.h"

using namespace R2D2;

bool File::fileExists(std::string path)
{
	struct stat buffer;
	return (stat (path.c_str(), &buffer) == 0);
}

std::string File::workingDirectory()
{
	return getcwd(NULL, 0);
}

std::vector<std::string> File::listFiles(std::string directory, bool recursive, std::string extension)
{
	std::vector<std::string> list;
	listFiles(list, directory, recursive, extension);
	return list;
}

void File::listFiles(std::vector<std::string>& list, std::string directory, bool recursive, std::string extension)
{
	if (StringEx::endsWith(directory, "/")) directory.pop_back();

	DIR* directoryPtr = opendir(directory.c_str());
	if (directoryPtr == NULL) return;

	struct dirent* item = NULL;
	while (true)
	{
		item = readdir(directoryPtr);
		if (item == NULL) break;

		if (item->d_type == DT_DIR)
		{
			if (strcmp(item->d_name, ".") == 0 || strcmp(item->d_name, "..") == 0) continue;

			Logging::log(LOG_DEBUG, "FILE", "Found directory: %s", &item->d_name);
			if (recursive) listFiles(list, directory + "/" + item->d_name, recursive, extension);

		}
		else if (item->d_type == DT_REG)
		{
			if (!extension.empty() && !StringEx::endsWith(item->d_name, extension)) continue;

			Logging::log(LOG_DEBUG, "FILE", "Found file: %s", &item->d_name);
			list.push_back(directory + "/" + item->d_name);
		}


	}
}
