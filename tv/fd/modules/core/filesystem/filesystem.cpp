#include "arl/utilities/filesystem.h"

// #include <boost/filesystem.hpp>

#include <algorithm>
#include <iostream>
#include <string>

#include "arl/utilities/logging.h"

#include <boost/filesystem.hpp>

namespace arl
{
bool fileExists(const std::string& path)
{
    if (access(path.c_str(), F_OK) != -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<std::string> getFileListFromDir(std::string dir)
{
    ASSERT(fileExists(dir)) << "Couldn't find directory!";

    std::vector<std::string> file_list;

    if (!dir.empty())
    {
        boost::filesystem::path boost_path(dir);
        boost::filesystem::recursive_directory_iterator end_iterator;

        for (boost::filesystem::recursive_directory_iterator it(boost_path); it != end_iterator;
             ++it)
        {
            const boost::filesystem::path file_path = (*it);
            file_list.push_back(file_path.string());
        }
    }

    return file_list;
}

std::vector<std::string> getSortedFileListFromDir(std::string dir)
{
    std::vector<std::string> file_list;

    if (!dir.empty())
    {
        struct list_entry
        {
            std::string name;
            int num;
        };

        std::vector<list_entry> combined_list;
        boost::filesystem::path boost_path(dir);
        boost::filesystem::recursive_directory_iterator end_iterator;

        for (boost::filesystem::recursive_directory_iterator it(boost_path); it != end_iterator;
             ++it)
        {
            const boost::filesystem::path file_path = (*it);

            std::string file_name = file_path.filename().string();
            int num = std::stoi(file_name, nullptr, 10);

            list_entry li = {file_path.string(), num};

            combined_list.push_back(li);
        }

        std::sort(combined_list.begin(), combined_list.end(), [](list_entry i, list_entry j) {
            return i.num < j.num;
        });

        for (auto le : combined_list)
        {
            file_list.push_back(le.name);
        }
    }

    return file_list;
}

std::string joinPaths(const std::string& file_path0, const std::string& file_path1)
{
    ASSERT(file_path0.length() > 0) << "file_path0 was empty!";
    ASSERT(file_path1.length() > 0) << "file_path1 was empty!";

    std::string joined_path =
        file_path0[file_path0.length() - 1] == '/' ? file_path0 : file_path0 + "/";
    joined_path = joined_path + file_path1;

    return joined_path;
}

std::string extractFileNameFromPath(const std::string& file_path)
{
    const int idx = file_path.find_last_of("/");
    const int total_length = file_path.length();
    return file_path.substr(idx + 1, total_length - idx - 1);
}

std::string extractPathFromFullFilePath(const std::string& full_file_path)
{
    // TODO: What happens if this is already a folder path?
    const int idx = full_file_path.find_last_of("/");
    if (idx == 0)
    {
        return "/";
    }
    else
    {
        return full_file_path.substr(0, idx);
    }
}

bool isDirectory(const std::string& path)
{
    return boost::filesystem::is_directory(path);
}

bool isFile(const std::string& path)
{
    return boost::filesystem::is_regular_file(path);
}

bool createDir(const std::string& path)
{
    return boost::filesystem::create_directory(path);
}

}  // namespace arl
