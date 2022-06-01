#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "arl/utilities/filesystem.h"
#include "arl/utilities/logging.h"
#include "arl/utilities/string.h"

#include "string_test_data.h"
#include "utils_test_utilities.h"

namespace arl
{
class StringUtilsTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(StringUtilsTest, TestJoinPathsNoSlash)
{
    const std::string path0 = "/path/to/a/folder";
    const std::string path1 = "another/path";
    const std::string joined_path_exp = path0 + "/" + path1;
    const std::string joined_path_act = joinPaths(path0, path1);
    ASSERT_EQ(joined_path_exp, joined_path_act);
}

TEST_F(StringUtilsTest, TestJoinPathsSlash)
{
    const std::string path0 = "/path/to/a/folder/";
    const std::string path1 = "another/path";
    const std::string joined_path_exp = path0 + path1;
    const std::string joined_path_act = joinPaths(path0, path1);
    ASSERT_EQ(joined_path_exp, joined_path_act);
}

TEST_F(StringUtilsTest, TestSplitString)
{
    const std::string s0 = "f4oim,g43rf,22f,hw4";
    const std::vector<std::string> split_str_exp = {"f4oim", "g43rf", "22f", "hw4"};
    const std::vector<std::string> split_str_act = splitString(s0, ",");

    ASSERT_EQ(split_str_exp.size(), split_str_act.size());
    ASSERT_STD_VEC_EQ_STD_VEC(split_str_exp, split_str_act);
}

TEST_F(StringUtilsTest, TestSplitString1)
{
    const std::string s0 = "f4oim,g43rf,22f,hw4,,";
    const std::vector<std::string> split_str_exp = {"f4oim", "g43rf", "22f", "hw4", "", ""};
    const std::vector<std::string> split_str_act = splitString(s0, ",");

    ASSERT_EQ(split_str_exp.size(), split_str_act.size());

    ASSERT_STD_VEC_EQ_STD_VEC(split_str_exp, split_str_act);
}

TEST_F(StringUtilsTest, TestFindSubStringIndices)
{
    const std::string s0 = "micdfacdeorcdmmacdfcdcdoacd";
    const std::string sub_str = "cd";

    const std::vector<size_t> indices_exp = {2, 6, 11, 16, 19, 21, 25};
    const std::vector<size_t> indices_act = findSubStringIndices(s0, sub_str);
    ASSERT_STD_VEC_EQ_STD_VEC(indices_exp, indices_act);
}

TEST_F(StringUtilsTest, TestFindSubStringIndicesEmpty)
{
    const std::string s0 = "micdfacdeorcdmmacdfcdcdoacd";
    const std::string sub_str = "cdq";

    const std::vector<size_t> indices_exp;
    const std::vector<size_t> indices_act = findSubStringIndices(s0, sub_str);
    ASSERT_EQ(indices_exp.size(), indices_act.size());
}

TEST_F(StringUtilsTest, TestFindSubStringIndices1)
{
    const std::vector<size_t> indices0 = findSubStringIndices("jd", "ade");
    ASSERT_EQ(indices0.size(), static_cast<size_t>(0));

    const std::vector<size_t> indices1 = findSubStringIndices("jdq", "jdq");
    ASSERT_EQ(indices1.size(), static_cast<size_t>(1));
    ASSERT_EQ(indices1[0], static_cast<size_t>(0));
}

TEST_F(StringUtilsTest, TestSplitStringOneSpace)
{
    const std::string s0 = " AaaABbbBCccC";
    const std::string s1 = "AaaA BbbBCccC";
    const std::string s2 = "AaaABbbB CccC";
    const std::string s3 = "AaaABbbBCccC ";
    const std::string s4 = "AaaABbbBCccC";
    const std::string delim = " ";
    const std::vector<std::string> split_string0 = splitString(s0, delim);
    const std::vector<std::string> split_string1 = splitString(s1, delim);
    const std::vector<std::string> split_string2 = splitString(s2, delim);
    const std::vector<std::string> split_string3 = splitString(s3, delim);
    const std::vector<std::string> split_string4 = splitString(s4, delim);

    const std::vector<std::string> split_string0_exp = {"", "AaaABbbBCccC"};
    const std::vector<std::string> split_string1_exp = {"AaaA", "BbbBCccC"};
    const std::vector<std::string> split_string2_exp = {"AaaABbbB", "CccC"};
    const std::vector<std::string> split_string3_exp = {"AaaABbbBCccC", ""};
    const std::vector<std::string> split_string4_exp = {"AaaABbbBCccC"};

    ASSERT_STD_VEC_EQ_STD_VEC(split_string0, split_string0_exp);
    ASSERT_STD_VEC_EQ_STD_VEC(split_string1, split_string1_exp);
    ASSERT_STD_VEC_EQ_STD_VEC(split_string2, split_string2_exp);
    ASSERT_STD_VEC_EQ_STD_VEC(split_string3, split_string3_exp);
    ASSERT_STD_VEC_EQ_STD_VEC(split_string4, split_string4_exp);
}

TEST_F(StringUtilsTest, TestSplitStringWithTestVectors)
{
    ASSERT_EQ(test_strings.size(), delimiters.size());
    ASSERT_EQ(gt_vectors.size(), delimiters.size());

    for (size_t k = 0; k < test_strings.size(); k++)
    {
        const std::string delim = delimiters[k];
        const std::string string_to_test = test_strings[k];

        const std::vector<std::string> split_string_act = splitString(string_to_test, delim);
        const std::vector<std::string> split_string_exp = gt_vectors[k];

        ASSERT_STD_VEC_EQ_STD_VEC(split_string_exp, split_string_act);
    }
}

TEST_F(StringUtilsTest, TestSortStringVector)
{
    const std::vector<std::vector<std::string>> v = {
        {"00001", "3", "6", "5", "2", "9", "0", "3", "-001", "3"},
        {"2", "1", "5", "4", "4", "2", "4"},
        {"-1", "-3", "4", "-3", "-3", "2", "-5", "6", "9"}};
    for (size_t k = 0; k < v.size(); k++)
    {
        const auto res = sortIntegerStringVector(v[k]);
        ASSERT_EQ(res.size(), v[k].size());
        for (size_t i = 0; i < (res.size() - 1); i++)
        {
            const int current_num = std::stoi(res[i], nullptr, 10);
            const int next_num = std::stoi(res[i + 1], nullptr, 10);
            ASSERT_GE(next_num, current_num);
        }
    }
}

TEST_F(StringUtilsTest, TestStripLeft)
{
    const std::vector<std::string> v_test = {
        ",,,HEEJ",
        "",
        ",",
        ",,,",
        "AB,,,,,",
        ",,,.",
        ".",
        "..",
        "...",
    };

    const std::vector<std::string> v_exp = {
        "HEEJ",
        "",
        "",
        "",
        "AB,,,,,",
        ".",
        ".",
        "..",
        "...",
    };
    const char char_to_strip = ',';

    for (size_t k = 0; k < v_test.size(); k++)
    {
        const std::string s_act = stripLeft(v_test[k], char_to_strip);
        ASSERT_EQ(v_exp[k], s_act);
    }
}

TEST_F(StringUtilsTest, TestStripRight)
{
    const std::vector<std::string> v_test = {
        "HEEJ,,,",
        "",
        ",",
        ",,,",
        ",,,,,AB",
        ".,,,",
        ".",
        "..",
        "...",
    };

    const std::vector<std::string> v_exp = {
        "HEEJ",
        "",
        "",
        "",
        ",,,,,AB",
        ".",
        ".",
        "..",
        "...",
    };
    const char char_to_strip = ',';

    for (size_t k = 0; k < v_test.size(); k++)
    {
        const std::string s_act = stripRight(v_test[k], char_to_strip);
        ASSERT_EQ(v_exp[k], s_act);
    }
}

TEST_F(StringUtilsTest, TestStripLeftRight)
{
    const std::vector<std::string> v_test = {",,.,,HEEJ,,,",
                                             "",
                                             ",",
                                             ",,",
                                             ".",
                                             "..",
                                             "...",
                                             ",,,",
                                             ",,,,,AB",
                                             "AB,,,,,",
                                             ".,,,",
                                             ",,,.",
                                             ".,,,.",
                                             ",,,.,,"};

    const std::vector<std::string> v_exp = {
        ".,,HEEJ", "", "", "", ".", "..", "...", "", "AB", "AB", ".", ".", ".,,,.", "."};
    const char char_to_strip = ',';

    for (size_t k = 0; k < v_test.size(); k++)
    {
        const std::string s_act = stripLeftRight(v_test[k], char_to_strip);
        ASSERT_EQ(v_exp[k], s_act);
    }
}

TEST_F(StringUtilsTest, TestSplitSeparatorList)
{
    const std::vector<std::string> list = {"aaa:frea",
                                           "qqqq:fmaero",
                                           "2q45t:gw4534q",
                                           "favbt:fre",
                                           "gh5wj:jht",
                                           "gw546t:5rfd",
                                           ":mfoiaer",
                                           "gtrer:"};

    std::vector<std::pair<std::string, std::string>> list_exp = {{"aaa", "frea"},
                                                                 {"qqqq", "fmaero"},
                                                                 {"2q45t", "gw4534q"},
                                                                 {"favbt", "fre"},
                                                                 {"gh5wj", "jht"},
                                                                 {"gw546t", "5rfd"},
                                                                 {"", "mfoiaer"},
                                                                 {"gtrer", ""}};

    std::vector<std::pair<std::string, std::string>> res = splitSeparatorList(list, ":", 0, 0);

    for (size_t k = 0; k < res.size(); k++)
    {
        const std::string key = res[k].first;
        const std::string val = res[k].second;

        ASSERT_EQ(key, list_exp[k].first);
        ASSERT_EQ(val, list_exp[k].second);
    }
}

TEST_F(StringUtilsTest, TestSplitSeparatorListWithFunction)
{
    const std::vector<std::string> list = {"aaa:frea",
                                           "qqqq:fmaero",
                                           "2q45t:gw4534q",
                                           "favbt:fre",
                                           "gh5wj:jht",
                                           "gw546t:5rfd",
                                           ":mfoiaer",
                                           "gtrer:"};

    std::vector<std::pair<std::string, std::string>> list_exp = {{"aaaKEY", "freaVAL"},
                                                                 {"qqqqKEY", "fmaeroVAL"},
                                                                 {"2q45tKEY", "gw4534qVAL"},
                                                                 {"favbtKEY", "freVAL"},
                                                                 {"gh5wjKEY", "jhtVAL"},
                                                                 {"gw546tKEY", "5rfdVAL"},
                                                                 {"KEY", "mfoiaerVAL"},
                                                                 {"gtrerKEY", "VAL"}};

    auto f_key = [](std::string s) -> std::string { return s + "KEY"; };
    auto f_val = [](std::string s) -> std::string { return s + "VAL"; };
    std::vector<std::pair<std::string, std::string>> res =
        splitSeparatorList(list, ":", f_key, f_val);

    for (size_t k = 0; k < res.size(); k++)
    {
        const std::string key = res[k].first;
        const std::string val = res[k].second;

        ASSERT_EQ(key, list_exp[k].first);
        ASSERT_EQ(val, list_exp[k].second);
    }
}

TEST_F(StringUtilsTest, TestSplitSeparatorListTemplated)
{
    const std::vector<std::string> list = {"53.5434:42",
                                           "6.23:5276",
                                           "6.56:345",
                                           "0.234:44",
                                           "6.324:794",
                                           "0.43242:123",
                                           "0.2345:746",
                                           "0.634:97"};

    std::vector<std::pair<float, int>> list_exp = {{53.5434f, 42},
                                                   {6.23f, 5276},
                                                   {6.56f, 345},
                                                   {0.234f, 44},
                                                   {6.324f, 794},
                                                   {0.43242f, 123},
                                                   {0.2345f, 746},
                                                   {0.634f, 97}};

    auto f_key = [](std::string s) -> float { return std::stof(s); };
    auto f_val = [](std::string s) -> int { return std::stoi(s); };
    std::vector<std::pair<float, int>> res =
        splitSeparatorListGeneral<float, int>(list, ":", f_key, f_val);

    for (size_t k = 0; k < res.size(); k++)
    {
        const auto key = res[k].first;
        const auto val = res[k].second;

        ASSERT_EQ(key, list_exp[k].first);
        ASSERT_EQ(val, list_exp[k].second);
    }
}

}  // namespace arl
