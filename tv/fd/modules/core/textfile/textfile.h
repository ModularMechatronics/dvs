#ifndef UTILITIES_TEXTFILE_READER_H_
#define UTILITIES_TEXTFILE_READER_H_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace arl
{
std::vector<std::string> readTextFileLines(const std::string &text_file_path);
std::vector<std::string> readTextFileLines(const std::string &folder_path,
                                           const std::string &file_name);
void writeTextFileLines(const std::vector<std::string> &lines_to_write,
                        const std::string &output_folder_path,
                        const std::string &output_file_name);

} // namespace arl

#endif
