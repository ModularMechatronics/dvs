#include "arl/utilities/textfile.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>

#include "arl/utilities/filesystem.h"
#include "arl/utilities/logging.h"
#include "arl/utilities/string.h"

namespace arl
{
std::vector<std::string> readTextFileLines(const std::string& text_file_path)
{
    ASSERT(boost::filesystem::exists(text_file_path))
        << "Couldn't find the specified text file: " << text_file_path;

    std::ifstream input_file(text_file_path);
    std::vector<std::string> file_lines;

    std::string line;

    while (std::getline(input_file, line))
    {
        file_lines.push_back(line);
    }

    return file_lines;
}

std::vector<std::string> readTextFileLines(const std::string& folder_path,
                                           const std::string& file_name)
{
    ASSERT(boost::filesystem::exists(folder_path))
        << "Couldn't find the specified folder path: " << folder_path;

    const std::string text_file_path = joinPaths(folder_path, file_name);

    ASSERT(boost::filesystem::exists(text_file_path))
        << "Couldn't find the specified text file: " << text_file_path;

    std::ifstream input_file(text_file_path);
    std::vector<std::string> file_lines;

    std::string line;

    while (std::getline(input_file, line))
    {
        file_lines.push_back(line);
    }

    return file_lines;
}

void writeTextFileLines(const std::vector<std::string>& lines_to_write,
                        const std::string& output_folder_path,
                        const std::string& output_file_name)
{
    ASSERT(boost::filesystem::exists(output_folder_path))
        << "Couldn't find the specified path: " << output_folder_path;

    std::string output_file_full_path = joinPaths(output_folder_path, output_file_name);

    std::ofstream output_file;
    output_file.open(output_file_full_path);
    for (std::string line : lines_to_write)
    {
        output_file << line + "\n";
    }
    output_file.close();
}

}  // namespace arl
