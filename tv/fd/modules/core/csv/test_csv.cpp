#include <unistd.h>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "arl/utilities/csv.h"
#include "arl/utilities/filesystem.h"
#include "arl/utilities/logging.h"
#include "arl/utilities/misc.h"

namespace arl
{
class CsvUtilsTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(CsvUtilsTest, TestWriteInt)
{
    const size_t num_rows = 4;

    std::vector<std::vector<int>> data(num_rows);
    for (size_t r = 0; r < num_rows; r++)
    {
        const size_t num_cols = r + 3;
        data[r].resize(num_cols);
        for (size_t c = 0; c < num_cols; c++)
        {
            data[r][c] = r + c * 30;
        }
    }
    writeIntCsvFile("./", "test_output.csv", data);

    std::vector<std::vector<int>> read_data = readIntCsvFile("./", "test_output.csv");

    for (size_t r = 0; r < num_rows; r++)
    {
        const size_t num_cols = r + 3;
        data[r].resize(num_cols);
        for (size_t c = 0; c < num_cols; c++)
        {
            ASSERT_EQ(read_data[r][c], data[r][c]);
        }
    }
}

TEST_F(CsvUtilsTest, TestWriteDouble)
{
    const size_t num_rows = 4;

    std::vector<std::vector<double>> data(num_rows);
    for (size_t r = 0; r < num_rows; r++)
    {
        const size_t num_cols = r + 3;
        data[r].resize(num_cols);
        for (size_t c = 0; c < num_cols; c++)
        {
            data[r][c] = r + c * 30;
        }
    }
    writeDoubleCsvFile("./", "test_output.csv", data);

    std::vector<std::vector<double>> read_data = readDoubleCsvFile("./", "test_output.csv");

    for (size_t r = 0; r < num_rows; r++)
    {
        const size_t num_cols = r + 3;
        data[r].resize(num_cols);
        for (size_t c = 0; c < num_cols; c++)
        {
            ASSERT_EQ(read_data[r][c], data[r][c]);
        }
    }
}

TEST_F(CsvUtilsTest, TestWriteString)
{
    const size_t num_rows = 4;

    std::vector<std::vector<std::string>> data(num_rows);
    for (size_t r = 0; r < num_rows; r++)
    {
        const size_t num_cols = r + 3;
        data[r].resize(num_cols);
        for (size_t c = 0; c < num_cols; c++)
        {
            data[r][c] = std::to_string(r + c * 30);
        }
    }
    writeStringCsvFile("./", "test_output.csv", data);

    std::vector<std::vector<std::string>> read_data = readStringCsvFile("./", "test_output.csv");

    for (size_t r = 0; r < num_rows; r++)
    {
        const size_t num_cols = r + 3;
        data[r].resize(num_cols);
        for (size_t c = 0; c < num_cols; c++)
        {
            ASSERT_EQ(read_data[r][c], data[r][c]);
        }
    }
}

}  // namespace arl
