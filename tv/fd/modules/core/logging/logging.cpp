#include "core/logging.h"

#include <sys/time.h>

#include <chrono>
#include <csignal>
#include <iostream>
#include <mutex>
#include <sstream>
#include <fstream>
#include <string>
#include <thread>

namespace timing
{
namespace internal
{
namespace
{
int64_t start_seconds;
int64_t stop_seconds;

int64_t start_micro_seconds;
int64_t stop_micro_seconds;

}

void startTimer()
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    start_seconds = current_time.tv_sec;
    start_micro_seconds = current_time.tv_usec;
}

void stopTimer()
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    stop_seconds = current_time.tv_sec;
    stop_micro_seconds = current_time.tv_usec;
}

}

}  // namespace timing

namespace logging
{
namespace internal
{

namespace
{

std::ostream* g_output_stream_ = &std::cout;
bool file_initialized_ = false;
std::ofstream g_logging_output_file_;

class FileCloser
{
public:

    FileCloser() {}
    ~FileCloser()
    {
        if(g_logging_output_file_.is_open())
        {
            g_logging_output_file_.close();
        }
    }

};

FileCloser g_file_closer_;

// #ifdef DEBUG

#define __loggingOutputStream (*g_output_stream_)

// #else

// #define __loggingOutputStream std::cout

// #endif

void flushToOutput(const std::string& s)
{
    // TODO: Lock mutex here?
    __loggingOutputStream << s << std::endl;
    if(g_logging_output_file_.is_open())
    {
        g_logging_output_file_ << s << std::endl;
    }
}

}

size_t getThreadId()
{
    return std::hash<std::thread::id>{}(std::this_thread::get_id());
}

std::string decNumberAsHexString(const size_t dec_number)
{
    std::stringstream str_stream;
    str_stream << std::hex << dec_number;
    return str_stream.str();
}

namespace
{

std::mutex g_mtx_;
bool g_use_colors_ = true;
bool g_show_file_ = true;
bool g_show_func_ = true;
bool g_show_line_number_ = true;
bool g_show_thread_id_ = true;

std::string getSeverityColor(const MessageSeverity msg_severity)
{
    std::string severity_string;
    switch (msg_severity)
    {
        case MessageSeverity::LOG_:
            severity_string = "\033[0m";
            break;
        case MessageSeverity::INFO:
            severity_string = "\033[32m";
            break;
        case MessageSeverity::DEBUG:
            severity_string = "\033[36m";
            break;
        case MessageSeverity::WARNING:
            severity_string = "\033[33m";
            break;
        case MessageSeverity::ERROR:
            severity_string = "\033[31m";
            break;
        case MessageSeverity::FATAL:
            severity_string = "\033[31m";
            break;
        case MessageSeverity::ASSERTION:
            severity_string = "\033[31m";
            break;
        case MessageSeverity::EXIT:
            severity_string = "\033[31m";
            break;
        default:
            severity_string = "UNKNOWNSEVERITYCOLOR";
    }
    return severity_string;
}

std::string getSeverityString(const MessageSeverity msg_severity)
{
    std::string severity_string;
    switch (msg_severity)
    {
        case MessageSeverity::LOG_:
            severity_string = "LOG";
            break;
        case MessageSeverity::INFO:
            severity_string = "INFO";
            break;
        case MessageSeverity::DEBUG:
            severity_string = "DEBUG";
            break;
        case MessageSeverity::WARNING:
            severity_string = "WARNING";
            break;
        case MessageSeverity::ERROR:
            severity_string = "ERROR";
            break;
        case MessageSeverity::FATAL:
            severity_string = "FATAL";
            break;
        case MessageSeverity::ASSERTION:
            severity_string = "ASSERTION FAILED";
            break;
        case MessageSeverity::EXIT:
            severity_string = "EXIT";
            break;
        default:
            severity_string = "UNKNOWNSEVERITY";
    }
    return severity_string;
}

std::string getResetColorString()
{
    return "\033[0m";
}

std::string getPreString(const MessageSeverity msg_severity,
                         const char* file_name,
                         const char* func_name,
                         const int line_number)
{
    std::lock_guard<std::mutex> l(internal::g_mtx_);

    const std::string severity_color = g_use_colors_ ? getSeverityColor(msg_severity) : "";
    const std::string reset_color = g_use_colors_ ? getResetColorString() : "";

    const std::string file_name_string = std::string(file_name);
    const std::string file_name_no_path = file_name_string.substr(file_name_string.find_last_of("/") + 1);

    const std::string func_string = g_show_func_ ? "[ " + std::string(func_name) + " ]" : "";

    const std::string file_string = g_show_file_ ? "[ " + file_name_no_path + " ]" : "";
    const std::string line_number_string = g_show_line_number_ ? "[ " + std::to_string(line_number) + " ]" : "";
    const std::string severity_string = "[ " + getSeverityString(msg_severity) + " ]";
    const std::string thread_id_string = g_show_thread_id_ ? "[ 0x" + decNumberAsHexString(getThreadId()) + " ]" : "";

    return severity_color + severity_string + thread_id_string + file_string + func_string + line_number_string + ": " +
           reset_color;
}

}

Log::Log(const MessageSeverity msg_severity,
                const char* file_name,
                const char* func_name,
                const int line_number)
    : pre_string_(getPreString(msg_severity, file_name, func_name, line_number)),
      assertion_condition_(true),
      call_exit_(false),
      should_print_(true),
      exit_code_(0)
{
}

Log::Log(const MessageSeverity msg_severity,
         const char* file_name,
         const char* func_name,
         const int line_number,
         const bool cond)
        : pre_string_(getPreString(msg_severity, file_name, func_name, line_number)),
          assertion_condition_(cond),
          call_exit_(false),
          should_print_(false),
          exit_code_(0)
{
}

Log::Log(const MessageSeverity msg_severity,
         const char* file_name,
         const char* func_name,
         const int line_number,
         const bool dummy,
         const int exit_code)
        : pre_string_(getPreString(msg_severity, file_name, func_name, line_number)),
          assertion_condition_(true),
          call_exit_(true),
          should_print_(false),
          exit_code_(exit_code)
{
}

Log::Log() : pre_string_(""), assertion_condition_(true), call_exit_(false), should_print_(true), exit_code_(0) {}

Log::Log(const bool cond) : pre_string_(""), assertion_condition_(true), call_exit_(false), should_print_(cond), exit_code_(0) {}

std::ostringstream& Log::getStream()
{
    return string_stream_;
}

Log::~Log()
{
    if (!assertion_condition_)
    {
        flushToOutput(pre_string_ + string_stream_.str());
        raise(SIGABRT);
    }
    else if(call_exit_)
    {
        flushToOutput(pre_string_ + string_stream_.str());
        exit(exit_code_);
    }
    else
    {
        if (should_print_)
        {
            flushToOutput(pre_string_ + string_stream_.str());
        }
    }
}

void setOutputStream(std::ostream* const strm)
{
    g_output_stream_ = strm;
}

}  // namespace internal

void openLoggingOutputFile(const std::string& file_name)
{
    internal::g_logging_output_file_.open(file_name);
}

void closeLoggingOutputFile()
{
    internal::g_logging_output_file_.close();
}

void useColors(const bool use_colors)
{
    std::lock_guard<std::mutex> l(internal::g_mtx_);
    internal::g_use_colors_ = use_colors;
}

void showFile(const bool show_file)
{
    std::lock_guard<std::mutex> l(internal::g_mtx_);
    internal::g_show_file_ = show_file;
}

void showLineNumber(const bool show_line_number)
{
    std::lock_guard<std::mutex> l(internal::g_mtx_);
    internal::g_show_func_ = show_line_number;
}

void showFunction(const bool show_function)
{
    std::lock_guard<std::mutex> l(internal::g_mtx_);
    internal::g_show_line_number_ = show_function;
}

void showThreadId(const bool show_thread_id)
{
    std::lock_guard<std::mutex> l(internal::g_mtx_);
    internal::g_show_thread_id_ = show_thread_id;
}

}  // namespace logging
