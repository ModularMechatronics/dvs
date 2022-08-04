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

namespace logging
{

void useColors(const bool use_colors);
void showFile(const bool show_file);
void showLineNumber(const bool show_line_number);
void showFunction(const bool show_function);
void showThreadId(const bool show_thread_id);

void openLoggingOutputFile(const std::string& file_name);
void closeLoggingOutputFile();

namespace internal
{

enum class MessageSeverity
{
    LOG_,
    INFO,
    DEBUG,
    WARNING,
    ERROR,
    FATAL,
    ASSERTION,
    EXIT
};

void setOutputStream(std::ostream* const strm);

class Log
{
public:
    explicit Log(const MessageSeverity msg_severity,
                 const char* file_name,
                 const char* func_name,
                 const int line_number);

    explicit Log(const MessageSeverity msg_severity,
                 const char* file_name,
                 const char* func_name,
                 const int line_number,
                 const bool cond);

    explicit Log(const MessageSeverity msg_severity,
                 const char* file_name,
                 const char* func_name,
                 const int line_number,
                 const bool dummy,
                 const int exit_code);

    explicit Log();

    explicit Log(const bool cond);

    std::ostringstream& getStream();

    ~Log();

private:
    std::ostringstream string_stream_;
    const std::string pre_string_;
    const bool assertion_condition_;
    const bool call_exit_;
    const bool should_print_;
    const int exit_code_;
};

size_t getThreadId();
std::string decNumberAsHexString(const size_t dec_number);

}  // namespace internal

}  // namespace logging

#define LOG \
    logging::internal::Log(logging::internal::MessageSeverity::LOG_, __FILE__, __func__, __LINE__).getStream()

#define LOG_INFO                                                                                   \
    logging::internal::Log(logging::internal::MessageSeverity::INFO, __FILE__, __func__, __LINE__).getStream()

#define LOG_DEBUG                                                                       \
    logging::internal::Log(logging::internal::MessageSeverity::DEBUG, __FILE__, __func__, __LINE__) \
        .getStream()

#define LOG_WARNING                                                                       \
    logging::internal::Log(logging::internal::MessageSeverity::WARNING, __FILE__, __func__, __LINE__) \
        .getStream()

#define LOG_ERROR                                                                       \
    logging::internal::Log(logging::internal::MessageSeverity::ERROR, __FILE__, __func__, __LINE__) \
        .getStream()

#define LOG_FATAL                                                                       \
    logging::internal::Log(logging::internal::MessageSeverity::FATAL, __FILE__, __func__, __LINE__) \
        .getStream()

#define PRINT logging::internal::Log().getStream()

#define PRINT_COND(cond) logging::internal::Log(cond).getStream()

#define ASSERT(cond)                                                             \
    logging::internal::Log(                                                      \
        logging::internal::MessageSeverity::ASSERTION, __FILE__, __func__, __LINE__, cond) \
        .getStream()

#define EXIT(exit_code)                                                                                  \
    logging::internal::Log(logging::internal::MessageSeverity::EXIT, __FILE__, __func__, __LINE__, false, exit_code) \
        .getStream()

#define TIC() timing::startTimer()

#define TOC_MS(msg)                                                                                          \
    {                                                                                                        \
        timing::stopTimer();                                                                                 \
        int64_t delta_seconds = timing::_Var_stop_seconds() - timing::_Var_start_seconds();                  \
        int64_t delta_microseconds = timing::_Var_stop_micro_seconds() - timing::_Var_start_micro_seconds(); \
        int64_t delta_time = delta_seconds * 1000000 + delta_microseconds;                                   \
        LOG_DEBUG() << msg << static_cast<float>(delta_time) / 1000.0f << " ms";                             \
    }

#define TOC_US(msg)                                                                                          \
    {                                                                                                        \
        timing::stopTimer();                                                                                 \
        int64_t delta_seconds = timing::_Var_stop_seconds() - timing::_Var_start_seconds();                  \
        int64_t delta_microseconds = timing::_Var_stop_micro_seconds() - timing::_Var_start_micro_seconds(); \
        int64_t delta_time = delta_seconds * 1000000 + delta_microseconds;                                   \
        LOG_DEBUG() << msg << delta_time << " us";                                                           \
    }

#endif // MODULES_CORE_LOGGING_LOGGING_H_
