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

std::string File::applicationDirectory()
{
	char buffer[PATH_MAX];
	if (readlink("/proc/self/exe", (char*)&buffer, PATH_MAX) == 0) return "";

	std::string path(buffer);
	path.erase(path.find_last_of('/'), path.length() - path.find_last_of('/'));

	return path;
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
	listContents(DT_REG, list, directory, recursive, extension);
}

void File::listLinks(std::vector<std::string>& list, std::string directory, bool recursive)
{
	listContents(DT_LNK, list, directory, recursive);
}

void File::listContents(unsigned int type, std::vector<std::string>& list, std::string directory, bool recursive, std::string extension)
{
	if (StringEx::endsWith(directory, "/")) directory.erase(directory.size()-1, 1);

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
		else if (type == DT_REG && item->d_type == DT_REG)
		{
			if (!extension.empty() && !StringEx::endsWith(item->d_name, extension)) continue;

			Logging::log(LOG_DEBUG, "FILE", "Found file: %s", &item->d_name);
			list.push_back(directory + "/" + item->d_name);
		}
		else if (type == DT_LNK && item->d_type == DT_LNK)
		{
			Logging::log(LOG_DEBUG, "FILE", "Found link: %s", &item->d_name);
			list.push_back(directory + "/" + item->d_name);
		}
	}
}
