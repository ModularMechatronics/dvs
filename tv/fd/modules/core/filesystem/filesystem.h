#ifndef UTILITIES_FILESYSTEM_UTILS_H_
#define UTILITIES_FILESYSTEM_UTILS_H_

#include <string>
#include <vector>

namespace arl
{
std::string joinPaths(const std::string& file_path0, const std::string& file_path1);
std::string extractFileNameFromPath(const std::string& file_path);
std::string extractPathFromFullFilePath(const std::string& full_file_path);
std::vector<std::string> getFileListFromDir(std::string dir);
std::vector<std::string> getSortedFileListFromDir(std::string dir);
bool fileExists(const std::string& path);
bool isDirectory(const std::string& path);
bool isFile(const std::string& path);

}  // namespace arl

#endif
