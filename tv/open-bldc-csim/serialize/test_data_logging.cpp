#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "data_logging.h"

class TestDataLogging : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(TestDataLogging, TestBasic)
{
    data_logging::append(0, 0.23f);
    data_logging::append(0, std::string("hello"));
    data_logging::append(0, "hello");

}

