#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "core/logging.h"

class TestLogging : public testing::Test
{
protected:
    std::ostringstream local_stream_;

    void SetUp() override {}

    void TearDown() override
    {
        logging::internal::setOutputStream(&std::cout);
    }
};

TEST_F(TestLogging, TestBasic)
{
    ASSERT_EQ(local_stream_.str(), "");
    logging::internal::setOutputStream(&local_stream_);

    // clang-format off
    PRINT << "PRINT";
    LOG << "LOG";                    const std::string log_line = std::to_string(__LINE__);
    LOG_INFO << "LOG_INFO";          const std::string log_info_line = std::to_string(__LINE__);
    LOG_DEBUG << "LOG_DEBUG";        const std::string log_debug_line = std::to_string(__LINE__);
    LOG_WARNING << "LOG_WARNING";    const std::string log_warning_line = std::to_string(__LINE__);
    LOG_ERROR << "LOG_ERROR";        const std::string log_error_line = std::to_string(__LINE__);
    LOG_FATAL << "LOG_FATAL";        const std::string log_fatal_line = std::to_string(__LINE__);
    // clang-format on

    const std::string thread_id_str = "0x" + logging::internal::decNumberAsHexString(logging::internal::getThreadId());

    const std::string gt = "PRINT\n"
    "\033[0m[ LOG ][ " + thread_id_str + " ][ test_logging.cpp ][ TestBody ][ " + log_line + " ]: \033[0mLOG\n"
    "\033[32m[ INFO ][ " + thread_id_str + " ][ test_logging.cpp ][ TestBody ][ " + log_info_line + " ]: \033[0mLOG_INFO\n"
    "\033[36m[ DEBUG ][ " + thread_id_str + " ][ test_logging.cpp ][ TestBody ][ " + log_debug_line + " ]: \033[0mLOG_DEBUG\n"
    "\033[33m[ WARNING ][ " + thread_id_str + " ][ test_logging.cpp ][ TestBody ][ " + log_warning_line + " ]: \033[0mLOG_WARNING\n"
    "\033[31m[ ERROR ][ " + thread_id_str + " ][ test_logging.cpp ][ TestBody ][ " + log_error_line + " ]: \033[0mLOG_ERROR\n"
    "\033[31m[ FATAL ][ " + thread_id_str + " ][ test_logging.cpp ][ TestBody ][ " + log_fatal_line + " ]: \033[0mLOG_FATAL\n";

    ASSERT_EQ(local_stream_.str(), gt);
}

TEST_F(TestLogging, TestPrintCond)
{
    ASSERT_EQ(local_stream_.str(), "");
    logging::internal::setOutputStream(&local_stream_);

    PRINT_COND(true) << "This should print";
    PRINT_COND(false) << "This should NOT print";

    const std::string gt = "This should print\n";

    ASSERT_EQ(local_stream_.str(), gt);
}

TEST_F(TestLogging, TestAssert)
{
    EXPECT_EXIT(ASSERT(false) << "This is an assertion from a test", ::testing::KilledBySignal(SIGABRT), "");
}

TEST_F(TestLogging, TestExit)
{
    EXPECT_EXIT(EXIT(0) << "This is an exit message from a test", testing::ExitedWithCode(0), "");
}

TEST_F(TestLogging, TestLoggingToFile)
{
    logging::openLoggingOutputFile("logging_file.txt");
}
