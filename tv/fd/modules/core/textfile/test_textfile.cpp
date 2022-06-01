#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "arl/utilities/filesystem.h"
#include "arl/utilities/logging.h"
#include "arl/utilities/string.h"
#include "arl/utilities/textfile.h"

namespace arl
{
class TextfileUtilsTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(TextfileUtilsTest, BasicTestReadWrite)
{
    /*const std::string file_path = "/Users/danielpihlquist/programming_projects/arl/build";
    const std::string file_name = "test_file.txt";

    std::vector<std::string> lines_to_write = {
        "hejsan", "vad heter", "    duuuuu   ", "vaaa", "heh"};
    arl::writeTextFileLines(lines_to_write, file_path, file_name);

    ASSERT_TRUE(fileExists(joinPaths(file_path, file_name)));

    std::vector<std::string> read_lines = arl::readTextFileLines(file_path, file_name);

    ASSERT_EQ(read_lines.size(), lines_to_write.size());
    for (size_t k = 0; k < read_lines.size(); k++)
    {
        ASSERT_EQ(read_lines[k], lines_to_write[k]);
    }*/
}

TEST_F(TextfileUtilsTest, ExtractFileNameAndPath)
{
    const std::string full_file_path0 = "/build/test_file.txt";

    const std::string file_name0 = extractFileNameFromPath(full_file_path0);
    const std::string file_path0 = extractPathFromFullFilePath(full_file_path0);

    ASSERT_EQ(file_name0, "test_file.txt");
    ASSERT_EQ(file_path0, "/build");

    const std::string file_name1 = extractFileNameFromPath("");
    const std::string file_path1 = extractPathFromFullFilePath("");

    const std::string file_name2 = extractFileNameFromPath("/tmp.txt");
    const std::string file_path2 = extractPathFromFullFilePath("/tmp.txt");

    ASSERT_EQ(file_name1, "");
    ASSERT_EQ(file_path1, "");

    ASSERT_EQ(file_name2, "tmp.txt");
    ASSERT_EQ(file_path2, "/");
}

std::string padWithZeros(const std::string& s, const size_t num_zeros)
{
    ASSERT(s.length() > 0) << "String can't have 0 length!";
    if (s.length() <= num_zeros)
    {
        return std::string(num_zeros - s.length() + 1, '0').append(s);
    }
    else
    {
        return s;
    }
}

TEST_F(TextfileUtilsTest, TestPadZeros)
{
    ASSERT_EQ("00001", padWithZeros("1", 4));
    ASSERT_EQ("00010", padWithZeros("10", 4));
    ASSERT_EQ("00100", padWithZeros("100", 4));
    ASSERT_EQ("01000", padWithZeros("1000", 4));
    ASSERT_EQ("10000", padWithZeros("10000", 4));
    ASSERT_EQ("100000", padWithZeros("100000", 4));
}

TEST_F(TextfileUtilsTest, SortingPaddedFiles)
{
    const std::string padded_path = "../test/unit_tests/utils/test_data/padded";
    const std::string non_padded_path = "../test/unit_tests/utils/test_data/non_padded";

    std::vector<std::string> padded_files = getSortedFileListFromDir(padded_path);
    std::vector<std::string> non_padded_files = getSortedFileListFromDir(non_padded_path);
    for (size_t k = 0; k < padded_files.size(); k++)
    {
        const std::string file_name = extractFileNameFromPath(padded_files[k]);
        ASSERT_EQ(file_name, padWithZeros(std::to_string(k), 3) + ".txt");
    }
    PRINT();
    for (size_t k = 0; k < non_padded_files.size(); k++)
    {
        const std::string file_name = extractFileNameFromPath(non_padded_files[k]);
        ASSERT_EQ(file_name, std::to_string(k) + ".txt");
    }
}

TEST_F(TextfileUtilsTest, GetFileList)
{
    const std::string non_padded_path = "../test/unit_tests/utils/test_data/non_padded";
    const size_t exp_num_files = 6;

    std::vector<std::string> files = getFileListFromDir(non_padded_path);

    ASSERT_EQ(files.size(), exp_num_files);

    for (size_t k = 0; k < exp_num_files; k++)
    {
        bool file_exists = false;
        const std::string exp_file_to_find = std::to_string(k) + ".txt";
        for (size_t i = 0; i < files.size(); i++)
        {
            if (extractFileNameFromPath(files[i]) == exp_file_to_find)
            {
                file_exists = true;
                break;
            }
        }
        ASSERT_TRUE(file_exists);
    }
}

/*TEST_F(TextfileUtilsTest, CurrentPath)
{
    auto getcwd = full_path(boost::filesystem::current_path());
}*/

}  // namespace arl
