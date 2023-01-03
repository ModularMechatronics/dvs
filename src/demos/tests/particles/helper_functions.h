#ifndef DEMOS_TESTS_PARTICLES_HELPER_FUNCTIONS_H_
#define DEMOS_TESTS_PARTICLES_HELPER_FUNCTIONS_H_

#include <stdint.h>

#include <string>
#include <vector>

namespace particles
{

std::vector<size_t> findSubStringIndices(const std::string& str, const std::string& sub_str)
{
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

std::vector<std::string> splitString(const std::string& string_to_split, const std::string& delimiter)
{
    std::vector<std::string> split_string;

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
            split_string[indices.size()] =
                string_to_split.substr(indices[indices.size() - 1] + delimiter.length(),
                                       string_to_split.length() - indices[indices.size() - 1]);
        }
    }

    return split_string;
}

}  // namespace particles

#endif
