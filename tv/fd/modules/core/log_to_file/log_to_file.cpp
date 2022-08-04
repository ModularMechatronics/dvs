#include "core/log_to_file.h"

#include <sys/time.h>

#include <chrono>
#include <csignal>
#include <iostream>
#include <mutex>
#include <sstream>
#include <fstream>
#include <string>
#include <thread>
#include <map>

namespace log_to_file
{
namespace internal
{

namespace
{

std::ofstream g_logging_output_file_;
std::map<std::string, std::ofstream*> g_output_files_;
std::mutex g_mtx_;


class FileCloser
{
public:

    FileCloser() {}
    ~FileCloser()
    {
        for (const auto& x : g_output_files_)
        {
            if(x.second->is_open())
            {
                x.second->close();
            }
        }
    }

};

FileCloser g_file_closer_;

void flushToOutput(const std::string& file_name, const std::string& s)
{
    std::lock_guard<std::mutex> l(internal::g_mtx_);
    if(g_output_files_.count(file_name) == 0)
    {
        g_output_files_[file_name] = new std::ofstream;
        g_output_files_[file_name]->open(file_name);
    }

    *(g_output_files_[file_name]) << s << std::endl;
}

}

LogToFile::LogToFile(const std::string& file_name)
    : file_name_(file_name)
{
}

std::ostringstream& LogToFile::getStream()
{
    return string_stream_;
}

LogToFile::~LogToFile()
{
    flushToOutput(file_name_, string_stream_.str());
}

}  // namespace internal

}  // namespace log_to_file
