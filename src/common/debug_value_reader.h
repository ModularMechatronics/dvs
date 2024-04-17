#ifndef DEBUG_VALUE_READER_H
#define DEBUG_VALUE_READER_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

namespace debug_value_reader
{

namespace internal
{
constexpr char* kDebugFilePath = "/Users/danielpi/.config/duoplot_debug_values.json";

inline bool& Variable_file_has_been_read()
{
    static bool file_has_been_read = false;

    return file_has_been_read;
}

inline nlohmann::json& Variable_json_data()
{
    static nlohmann::json json_data{};

    return json_data;
}

inline void readFile()
{
    if (!internal::Variable_file_has_been_read())
    {
        nlohmann::json& json_data = internal::Variable_json_data();

        std::ifstream input_file(internal::kDebugFilePath);
        nlohmann::json j;

        input_file >> json_data;
        internal::Variable_file_has_been_read() = true;
    }
}

}  // namespace internal

template <typename T> T getValue(const std::string& key)
{
    static_assert(std::is_same<T, double>::value || std::is_same<T, float>::value ||
                      std::is_same<T, std::int64_t>::value || std::is_same<T, std::uint64_t>::value ||
                      std::is_same<T, std::int32_t>::value || std::is_same<T, std::uint32_t>::value ||
                      std::is_same<T, std::int16_t>::value || std::is_same<T, std::uint16_t>::value ||
                      std::is_same<T, std::int8_t>::value || std::is_same<T, std::uint8_t>::value ||
                      std::is_same<T, bool>::value || std::is_same<T, std::string>::value,
                  "Type not supported!");
    internal::readFile();

    nlohmann::json& json_data = internal::Variable_json_data();

    if (!json_data.contains(key))
    {
        throw std::runtime_error("Key \"" + key + "\" not found");
    }

    return json_data[key].get<T>();
}

template <typename T> T getValue(const std::string& key, const T& default_value)
{
    static_assert(std::is_same<T, double>::value || std::is_same<T, float>::value ||
                      std::is_same<T, std::int64_t>::value || std::is_same<T, std::uint64_t>::value ||
                      std::is_same<T, std::int32_t>::value || std::is_same<T, std::uint32_t>::value ||
                      std::is_same<T, std::int16_t>::value || std::is_same<T, std::uint16_t>::value ||
                      std::is_same<T, std::int8_t>::value || std::is_same<T, std::uint8_t>::value ||
                      std::is_same<T, bool>::value || std::is_same<T, std::string>::value,
                  "Type not supported!");
    internal::readFile();

    nlohmann::json& json_data = internal::Variable_json_data();

    if (!json_data.contains(key))
    {
        return default_value;
    }

    return json_data[key].get<T>();
}

}  // namespace debug_value_reader

#endif  // DEBUG_VALUE_READER_H
