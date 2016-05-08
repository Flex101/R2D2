#ifndef FILE_H
#define FILE_H

#include <string>
#include <vector>

namespace R2D2 {

class File
{
public:
	static bool fileExists(std::string path);
	static std::string applicationDirectory();
	static std::string workingDirectory();

	static std::vector<std::string> listFiles(std::string directory, bool recursive = false, std::string extension = "");
	static void listFiles(std::vector<std::string>& list, std::string directory, bool recursive = false, std::string extension = "");
	static void listLinks(std::vector<std::string>& list, std::string directory, bool recursive = false);

protected:
	static void listContents(unsigned int type, std::vector<std::string>& list, std::string directory, bool recursive = false, std::string extension = "");
};

}// namespace R2D2

#endif // FILE_H
