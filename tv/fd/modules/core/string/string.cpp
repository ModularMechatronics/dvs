#include "arl/utilities/string.h"

#include <algorithm>
#include <iostream>
#include <string>

#include "arl/utilities/logging.h"

namespace arl
{
std::vector<std::pair<std::string, std::string>> splitSeparatorList(
    const std::vector<std::string>& string_list,
    const std::string& separator,
    std::function<std::string(std::string)> key_function,
    std::function<std::string(std::string)> val_function)
{
    std::vector<std::pair<std::string, std::string>> pv;

    for (size_t k = 0; k < string_list.size(); k++)
    {
        const std::string current_string = string_list[k];
        const size_t idx_of_first = current_string.find(separator);
        ASSERT(idx_of_first != current_string.npos)
            << "No separator found in string " << current_string;

        const std::string key = current_string.substr(0, idx_of_first);
        const std::string val =
            current_string.substr(idx_of_first + 1, current_string.length() - idx_of_first - 1);

        const std::string key_mod = key_function == NULL ? key : key_function(key);
        const std::string val_mod = val_function == NULL ? val : val_function(val);

        std::pair<std::string, std::string> p(key_mod, val_mod);
        pv.push_back(p);
    }

    return pv;
}

std::string stripLeft(const std::string& s, const char char_to_strip)
{
    if (s.length() == 0)
    {
        return "";
    }
    else
    {
        size_t idx = 0;
        while ((idx < s.length()) && (s[idx] == char_to_strip))
        {
            idx++;
        }
        return s.substr(idx, s.length() - idx);
    }
}

std::string stripRight(const std::string& s, const char char_to_strip)
{
    if (s.length() == 0)
    {
        return "";
    }
    else
    {
        int idx = s.length() - 1;
        while ((idx >= 0) && (s[idx] == char_to_strip))
        {
            idx--;
        }
        return s.substr(0, idx + 1);
    }
}

std::string stripLeftRight(const std::string& s, const char char_to_strip)
{
    if (s.length() == 0)
    {
        return "";
    }
    else
    {
        int right_idx = s.length() - 1, left_idx = 0;

        while ((right_idx >= 0) && (s[right_idx] == char_to_strip))
        {
            right_idx--;
        }

        if (right_idx < 0)
        {
            return "";
        }

        while ((static_cast<size_t>(left_idx) < s.length()) && (s[left_idx] == char_to_strip))
        {
            left_idx++;
        }

        if (static_cast<size_t>(right_idx) == (s.length() - 1) && (left_idx == 0))
        {
            return s;
        }

        return s.substr(left_idx, right_idx - left_idx + 1);
    }
}

std::vector<size_t> findSubStringIndices(const std::string& str, const std::string& sub_str)
{
    ASSERT(sub_str.length() > 0) << "Can not find empty substring!";
    std::vector<size_t> res_vec;

    if (str.size() > 0)
    {
        const char first_char = sub_str[0];
        if (sub_str.length() == 1)
        {
            for (size_t k = 0; k < str.length(); k++)
            {
                if (str[k] == first_char)
                {
                    res_vec.push_back(k);
                }
            }
        }
        else if (str.length() >= sub_str.length())
        {
            for (size_t k = 0; k < (str.length() + 1 - sub_str.length()); k++)
            {
                if (str[k] == first_char)
                {
                    const std::string current_sub_str = str.substr(k, sub_str.length());
                    if (current_sub_str == sub_str)
                    {
                        res_vec.push_back(k);
                    }
                }
            }
        }
    }
    return res_vec;
}

std::vector<std::string> splitString(const std::string& string_to_split,
                                     const std::string& delimiter)
{
    ASSERT(delimiter.size()) << "Can't have zero size delimiter!";

    std::vector<std::string> split_string;

    if (string_to_split.length() == 0)
    {
        split_string.push_back("");
    }
    else if (string_to_split.length() == 1)
    {
        if (delimiter.length() > string_to_split.length())
        {
            split_string.push_back(string_to_split);
        }
        else  // Lengths are equal (delimiter can't have length 0)
        {
            if (string_to_split == delimiter)
            {
                split_string.push_back("");
                split_string.push_back("");
            }
            else
            {
                split_string.push_back(string_to_split);
            }
        }
    }
    else
    {
        if (delimiter.length() > string_to_split.length())
        {
            split_string.push_back(string_to_split);
        }
        else if (delimiter.length() == string_to_split.length())
        {
            if (string_to_split == delimiter)
            {
                split_string.push_back("");
                split_string.push_back("");
            }
            else
            {
                split_string.push_back(string_to_split);
            }
        }
        else
        {
            const std::vector<size_t> indices = findSubStringIndices(string_to_split, delimiter);
            if (indices.size() == 0)
            {
                split_string.push_back(string_to_split);
            }
            else
            {
                split_string.resize(indices.size() + 1);
                if (indices[0] == 0)
                {
                    split_string[0] = "";
                }
                else
                {
                    split_string[0] = string_to_split.substr(0, indices[0]);
                }

                for (size_t k = 0; k < (indices.size() - 1); k++)
                {
                    const size_t idx0 = indices[k] + delimiter.length();
                    const size_t idx1 = indices[k + 1];

                    split_string[k + 1] = string_to_split.substr(idx0, idx1 - idx0);
                }

                if (indices[indices.size() - 1] == (string_to_split.length() - 1))
                {
                    split_string[indices.size()] = "";
                }
                else
                {
                    split_string[indices.size()] = string_to_split.substr(
                        indices[indices.size() - 1] + delimiter.length(),
                        string_to_split.length() - indices[indices.size() - 1]);
                }
            }
        }
    }

    return split_string;
}

std::vector<std::string> sortIntegerStringVector(const std::vector<std::string>& input_vector)
{
    // Sorts a std::vector<std::string> on the form input_vector = {"2", "1", "3", "0", "4"}
    std::vector<std::string> output_vector = input_vector;

    auto predicate_func = [](std::string s0, std::string s1) -> bool {
        const int i0 = std::stoi(s0, nullptr, 10);
        const int i1 = std::stoi(s1, nullptr, 10);
        return i0 < i1;
    };
    std::sort(output_vector.begin(), output_vector.end(), predicate_func);

    return output_vector;
}

}  // namespace arl
