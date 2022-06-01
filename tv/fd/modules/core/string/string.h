#ifndef UTILITIES_STRING_UTILS_H_
#define UTILITIES_STRING_UTILS_H_

#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace arl
{
std::vector<std::string> splitString(const std::string& string_to_split,
                                     const std::string& delimiter);
std::vector<size_t> findSubStringIndices(const std::string& str, const std::string& sub_str);

std::vector<std::string> sortIntegerStringVector(const std::vector<std::string>& input_vector);
std::string stripLeft(const std::string& s, const char char_to_strip);
std::string stripRight(const std::string& s, const char char_to_strip);
std::string stripLeftRight(const std::string& s, const char char_to_strip);

std::vector<std::pair<std::string, std::string>> splitSeparatorList(
    const std::vector<std::string>& string_list,
    const std::string& separator,
    std::function<std::string(std::string)> key_function = NULL,
    std::function<std::string(std::string)> val_function = NULL);

template <typename KeyT, typename ValT>
std::vector<std::pair<KeyT, ValT>> splitSeparatorListGeneral(
    const std::vector<std::string>& string_list,
    const std::string& separator,
    std::function<KeyT(const std::string&)> key_function,
    std::function<ValT(const std::string&)> val_function)
{
    std::vector<std::pair<KeyT, ValT>> pv;

    for (size_t k = 0; k < string_list.size(); k++)
    {
        const std::string current_string = string_list[k];
        const size_t idx_of_first = current_string.find(separator);
        assert(idx_of_first != current_string.npos && "No separator found in string!");

        const std::string key = current_string.substr(0, idx_of_first);
        const std::string val =
            current_string.substr(idx_of_first + 1, current_string.length() - idx_of_first - 1);
        assert(key.length() > 0 && "Key string length is 0!");
        assert(val.length() > 0 && "Val string length is 0!");

        std::pair<KeyT, ValT> p(key_function(key), val_function(val));
        pv.push_back(p);
    }

    return pv;
}

}  // namespace arl

#endif
