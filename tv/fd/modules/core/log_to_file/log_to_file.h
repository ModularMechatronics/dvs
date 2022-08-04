#ifndef MODULES_CORE_LOGGING_LOGGING_H_
#define MODULES_CORE_LOGGING_LOGGING_H_

#include <sys/time.h>

#include <chrono>
#include <csignal>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

namespace log_to_file
{

// void openLoggingOutputFile(const std::string& file_name);
// void closeLoggingOutputFile();

namespace internal
{


class LogToFile
{
public:
    explicit LogToFile(const std::string& file_name);

    LogToFile() = delete;

    std::ostringstream& getStream();

    ~LogToFile();

private:
    std::ostringstream string_stream_;
    const std::string file_name_;
};

}  // namespace internal

}  // namespace log_to_file

#define LOG_TO_FILE(file_name) log_to_file::internal::LogToFile(file_name).getStream()

#endif // MODULES_CORE_LOGGING_LOGGING_H_
