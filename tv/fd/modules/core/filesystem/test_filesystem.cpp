#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "arl/utilities/filesystem.h"
#include "arl/utilities/logging.h"
#include "arl/utilities/string.h"

namespace arl
{
class FilesystemUtilsTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(FilesystemUtilsTest, TestIsDir)
{
    const std::string dir_path = "../test/unit_tests/utils/test_data/a_directory";
    const std::string file_path = "../test/unit_tests/utils/test_data/a_directory/a_file";

    ASSERT_TRUE(isDirectory(dir_path));
    ASSERT_FALSE(isDirectory(file_path));

    ASSERT_FALSE(isFile(dir_path));
    ASSERT_TRUE(isFile(file_path));
}

TEST_F(FilesystemUtilsTest, TestFileExists)
{
    const std::string dir_path = "../test/unit_tests/utils/test_data/a_directory";
    const std::string file_path = "../test/unit_tests/utils/test_data/a_directory/a_file";

    ASSERT_TRUE(fileExists(dir_path));
    ASSERT_TRUE(fileExists(file_path));

    ASSERT_FALSE(fileExists(dir_path + "something_else"));
    ASSERT_FALSE(fileExists(file_path + "something_else"));
}

}  // namespace arl
