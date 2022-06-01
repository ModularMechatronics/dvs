#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "core/log_to_file.h"

namespace
{
std::vector<std::string> readTextFileLines(const std::string& text_file_path)
{
    std::ifstream input_file(text_file_path);
    std::vector<std::string> file_lines;

    std::string line;

    while (std::getline(input_file, line))
    {
        file_lines.push_back(line);
    }

    return file_lines;
}
}

TEST(TestLogToFile, TestBasic)
{
    const size_t num_lines = 10;
    for(size_t k = 0; k < num_lines; k++)
    {
        const float kf = k;
        LOG_TO_FILE("test_file0") << "hejsan" << std::to_string(k);
        LOG_TO_FILE("test_file1") << "hejsan" << std::to_string(k * 2 + 1);
        LOG_TO_FILE("test_file2") << kf * 3.41523423f + 0.523f << "," << kf / 2.4125 + 0.14f;
    }

    const std::vector<std::string> tf0_lines = readTextFileLines("test_file0");
    const std::vector<std::string> tf1_lines = readTextFileLines("test_file1");
    const std::vector<std::string> tf2_lines = readTextFileLines("test_file2");

    ASSERT_EQ(num_lines, tf0_lines.size());
    ASSERT_EQ(num_lines, tf1_lines.size());
    ASSERT_EQ(num_lines, tf2_lines.size());

    for(size_t k = 0; k < num_lines; k++)
    {
        const float kf = k;
        ASSERT_EQ("hejsan" + std::to_string(k), tf0_lines[k]);
        ASSERT_EQ("hejsan" + std::to_string(k * 2 + 1), tf1_lines[k]);

        const std::string s0 = tf2_lines[k].substr(0, tf2_lines[k].find(","));
        const std::string s1 = tf2_lines[k].substr(tf2_lines[k].find(",") + 1);

        ASSERT_NEAR(std::stof(s0), kf * 3.41523423f + 0.523f, 1e-4f);
        ASSERT_NEAR(std::stof(s1), kf / 2.4125 + 0.14f, 1e-4f);
    }



}
