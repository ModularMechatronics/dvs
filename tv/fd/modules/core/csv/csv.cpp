#include "arl/utilities/csv.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>

#include "arl/utilities/logging.h"
#include "arl/utilities/string.h"
#include "arl/utilities/textfile.h"

namespace arl
{
void writeIntCsvFile(const std::string& output_folder,
                     const std::string& output_file_name,
                     const std::vector<std::vector<int>>& data)
{
    std::vector<std::string> data_string_lines;
    for (size_t r = 0; r < data.size(); r++)
    {
        const std::vector<int>& data_vec = data[r];
        std::string s = "";
        for (size_t c = 0; c < data_vec.size(); c++)
        {
            s = s + std::to_string(data_vec[c]);
            if (c != (data_vec.size() - 1))
            {
                s = s + ",";
            }
        }
        data_string_lines.push_back(s);
    }

    writeTextFileLines(data_string_lines, output_folder, output_file_name);
}

std::vector<std::vector<int>> readIntCsvFile(const std::string& folder_path,
                                             const std::string& file_name)
{
    std::vector<std::string> read_lines = readTextFileLines(folder_path, file_name);

    std::vector<std::vector<int>> read_data;

    for (size_t r = 0; r < read_lines.size(); r++)
    {
        std::vector<int> row_data;
        const std::string& row_string = read_lines[r];
        std::vector<std::string> split_string = splitString(row_string, ",");
        for (const std::string& sub_string : split_string)
        {
            if (sub_string.length() == 0)
            {
                row_data.push_back(std::numeric_limits<int>::max());
            }
            else
            {
                row_data.push_back(std::stoi(sub_string));
            }
        }

        read_data.push_back(row_data);
    }
    return read_data;
}

void writeDoubleCsvFile(const std::string& output_folder,
                        const std::string& output_file_name,
                        const std::vector<std::vector<double>>& data)
{
    std::vector<std::string> data_string_lines;
    for (size_t r = 0; r < data.size(); r++)
    {
        const std::vector<double>& data_vec = data[r];
        std::string s = "";
        for (size_t c = 0; c < data_vec.size(); c++)
        {
            s = s + std::to_string(data_vec[c]);
            if (c != (data_vec.size() - 1))
            {
                s = s + ",";
            }
        }
        data_string_lines.push_back(s);
    }

    writeTextFileLines(data_string_lines, output_folder, output_file_name);
}

std::vector<std::vector<double>> readDoubleCsvFile(const std::string& folder_path,
                                                   const std::string& file_name)
{
    std::vector<std::string> read_lines = readTextFileLines(folder_path, file_name);

    std::vector<std::vector<double>> read_data;

    for (size_t r = 0; r < read_lines.size(); r++)
    {
        std::vector<double> row_data;
        const std::string& row_string = read_lines[r];
        std::vector<std::string> split_string = splitString(row_string, ",");
        for (const std::string& sub_string : split_string)
        {
            if (sub_string.length() == 0)
            {
                row_data.push_back(NAN);
            }
            else
            {
                row_data.push_back(std::stod(sub_string));
            }
        }

        read_data.push_back(row_data);
    }
    return read_data;
}

void writeStringCsvFile(const std::string& output_folder,
                        const std::string& output_file_name,
                        const std::vector<std::vector<std::string>>& data)
{
    std::vector<std::string> data_string_lines;
    for (size_t r = 0; r < data.size(); r++)
    {
        const std::vector<std::string>& data_vec = data[r];
        std::string s = "";
        for (size_t c = 0; c < data_vec.size(); c++)
        {
            s = s + data_vec[c];
            if (c != (data_vec.size() - 1))
            {
                s = s + ",";
            }
        }
        data_string_lines.push_back(s);
    }

    writeTextFileLines(data_string_lines, output_folder, output_file_name);
}

std::vector<std::vector<std::string>> readStringCsvFile(const std::string& folder_path,
                                                        const std::string& file_name)
{
    const std::vector<std::string> read_lines = readTextFileLines(folder_path, file_name);

    std::vector<std::vector<std::string>> read_data;

    for (size_t r = 0; r < read_lines.size(); r++)
    {
        std::vector<std::string> row_data;
        const std::string& row_string = read_lines[r];
        std::vector<std::string> split_string = splitString(row_string, ",");
        for (const std::string& sub_string : split_string)
        {
            row_data.push_back(sub_string);
        }

        read_data.push_back(row_data);
    }
    return read_data;
}

}  // namespace arl
