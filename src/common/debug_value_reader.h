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
constexpr char* kDebugFilePath = "/Users/danielpi/.config/dvs_debug_values.json";

inline bool& _Var_file_has_been_read()
{
    static bool file_has_been_read = false;

    return file_has_been_read;
}

inline nlohmann::json& _Var_json_data()
{
    static nlohmann::json json_data{};

    return json_data;
}

inline void readFile()
{
    if (!internal::_Var_file_has_been_read())
    {
        nlohmann::json& json_data = internal::_Var_json_data();

        std::ifstream input_file(internal::kDebugFilePath);
        nlohmann::json j;

        input_file >> json_data;
        internal::_Var_file_has_been_read() = true;
    }
}

}  // namespace internal

inline std::string readString(const std::string& key)
{
    internal::readFile();

    nlohmann::json& json_data = internal::_Var_json_data();

    return json_data[key].get<std::string>();
}

inline double readDouble(const std::string& key)
{
    internal::readFile();

    nlohmann::json& json_data = internal::_Var_json_data();

    return json_data[key].get<double>();
}

inline float readFloat(const std::string& key)
{
    return static_cast<float>(readDouble(key));
}

inline std::int64_t readInt64(const std::string& key)
{
    internal::readFile();

    nlohmann::json& json_data = internal::_Var_json_data();

    return json_data[key].get<std::int64_t>();
}

inline std::uint64_t readUInt64(const std::string& key)
{
    internal::readFile();

    nlohmann::json& json_data = internal::_Var_json_data();

    return json_data[key].get<std::uint64_t>();
}

inline std::int32_t readInt32(const std::string& key)
{
    return static_cast<std::int32_t>(readInt64(key));
}

inline std::uint32_t readUInt32(const std::string& key)
{
    return static_cast<std::uint32_t>(readUInt64(key));
}

inline std::int16_t readInt16(const std::string& key)
{
    return static_cast<std::int16_t>(readInt64(key));
}

inline std::uint16_t readUInt16(const std::string& key)
{
    return static_cast<std::uint16_t>(readUInt64(key));
}

inline std::int8_t readInt8(const std::string& key)
{
    return static_cast<std::int8_t>(readInt64(key));
}

inline std::uint8_t readUInt8(const std::string& key)
{
    return static_cast<std::uint8_t>(readUInt64(key));
}

}  // namespace debug_value_reader

#endif  // DEBUG_VALUE_READER_H
