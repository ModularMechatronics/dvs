#ifndef DEMOS_ARGS_H
#define DEMOS_ARGS_H

#include <cstdint>
#include <map>
#include <string>
#include <type_traits>

namespace debug_value_args
{

namespace internal
{

std::map<std::string, std::string>& getParsedArgs()
{
    static std::map<std::string, std::string> parsed_args;
    return parsed_args;
}

std::string getArgumentNameWithNoLeadingDashes(const std::string& arg_name)
{
    if (arg_name[0] == '-')
    {
        if (arg_name[1] == '-')
        {
            return arg_name.substr(2);
        }
        else
        {
            return arg_name.substr(1);
        }
    }
    else
    {
        return arg_name;
    }
}
}  // namespace internal

inline void parseArgs(int argc, char* argv[])
{
    std::map<std::string, std::string>& parsed_args = internal::getParsedArgs();

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg[0] == '-')
        {
            if (arg[1] == '-')
            {
                arg = arg.substr(2);
            }
            else
            {
                arg = arg.substr(1);
            }

            std::string arg_name = arg;
            if (i + 1 < argc)
            {
                std::string arg_value = argv[i + 1];
                if (arg_value[0] != '-')
                {
                    i++;
                    parsed_args[arg_name] = arg_value;
                }
                else
                {
                    parsed_args[arg_name] = "";
                }
            }
            else
            {
                parsed_args[arg_name] = "";
            }
        }
    }
}

bool isFlagArgumentPresent(const std::string& arg_name)
{
    if (arg_name.empty())
    {
        throw std::runtime_error("Argument name cannot be empty");
    }

    const std::string new_arg_name = internal::getArgumentNameWithNoLeadingDashes(arg_name);

    std::map<std::string, std::string>& parsed_args = internal::getParsedArgs();

    return parsed_args.find(new_arg_name) != parsed_args.end();
}

template <typename T> T getValue(const std::string& arg_name)
{
    static_assert(std::is_same<T, double>::value || std::is_same<T, float>::value ||
                      std::is_same<T, std::int64_t>::value || std::is_same<T, std::uint64_t>::value ||
                      std::is_same<T, std::int32_t>::value || std::is_same<T, std::uint32_t>::value ||
                      std::is_same<T, std::int16_t>::value || std::is_same<T, std::uint16_t>::value ||
                      std::is_same<T, std::int8_t>::value || std::is_same<T, std::uint8_t>::value,
                  "Type not supported!");

    if (arg_name.empty())
    {
        throw std::runtime_error("Argument name cannot be empty");
    }

    const std::string new_arg_name = internal::getArgumentNameWithNoLeadingDashes(arg_name);

    std::map<std::string, std::string>& parsed_args = internal::getParsedArgs();

    if (parsed_args.find(new_arg_name) == parsed_args.end())
    {
        throw std::runtime_error("Argument " + new_arg_name + " not found");
    }

    if (parsed_args[new_arg_name].empty())
    {
        throw std::runtime_error("Argument " + new_arg_name + " has no value");
    }

    if (std::is_same<T, float>::value)
    {
        return std::stof(parsed_args[new_arg_name]);
    }
    else if (std::is_same<T, double>::value)
    {
        return std::stod(parsed_args[new_arg_name]);
    }
    else if (std::is_same<T, std::int64_t>::value)
    {
        return std::stoll(parsed_args[new_arg_name]);
    }
    else if (std::is_same<T, std::uint64_t>::value)
    {
        return std::stoull(parsed_args[new_arg_name]);
    }
    else if (std::is_same<T, std::int32_t>::value)
    {
        return std::stoi(parsed_args[new_arg_name]);
    }
    else if (std::is_same<T, std::uint32_t>::value)
    {
        return std::stoul(parsed_args[new_arg_name]);
    }
    else if (std::is_same<T, std::int16_t>::value)
    {
        return std::stoi(parsed_args[new_arg_name]);
    }
    else if (std::is_same<T, std::uint16_t>::value)
    {
        return std::stoul(parsed_args[new_arg_name]);
    }
    else if (std::is_same<T, std::int8_t>::value)
    {
        return std::stoi(parsed_args[new_arg_name]);
    }
    else if (std::is_same<T, std::uint8_t>::value)
    {
        return std::stoul(parsed_args[new_arg_name]);
    }
    else
    {
        throw std::runtime_error("Unsupported type");
    }
}

template <> bool getValue(const std::string& arg_name)
{
    if (arg_name.empty())
    {
        throw std::runtime_error("Argument name cannot be empty");
    }

    const std::string new_arg_name = internal::getArgumentNameWithNoLeadingDashes(arg_name);

    std::map<std::string, std::string>& parsed_args = internal::getParsedArgs();

    if (parsed_args.find(new_arg_name) == parsed_args.end())
    {
        throw std::runtime_error("Argument " + new_arg_name + " not found");
    }

    if (parsed_args[new_arg_name].empty())
    {
        throw std::runtime_error("Argument " + new_arg_name + " has no value");
    }

    return parsed_args[new_arg_name] == "true" || parsed_args[new_arg_name] == "1" ||
           parsed_args[new_arg_name] == "True";
}

template <> std::string getValue(const std::string& arg_name)
{
    if (arg_name.empty())
    {
        throw std::runtime_error("Argument name cannot be empty");
    }

    const std::string new_arg_name = internal::getArgumentNameWithNoLeadingDashes(arg_name);

    std::map<std::string, std::string>& parsed_args = internal::getParsedArgs();

    if (parsed_args.find(new_arg_name) == parsed_args.end())
    {
        throw std::runtime_error("Argument " + new_arg_name + " not found");
    }

    if (parsed_args[new_arg_name].empty())
    {
        throw std::runtime_error("Argument " + new_arg_name + " has no value");
    }

    return parsed_args[new_arg_name];
}

template <typename T> T getValue(const std::string& arg_name, const T& default_value)
{
    static_assert(std::is_same<T, double>::value || std::is_same<T, float>::value ||
                      std::is_same<T, std::int64_t>::value || std::is_same<T, std::uint64_t>::value ||
                      std::is_same<T, std::int32_t>::value || std::is_same<T, std::uint32_t>::value ||
                      std::is_same<T, std::int16_t>::value || std::is_same<T, std::uint16_t>::value ||
                      std::is_same<T, std::int8_t>::value || std::is_same<T, std::uint8_t>::value,
                  "Type not supported!");

    if (arg_name.empty())
    {
        throw std::runtime_error("Argument name cannot be empty");
    }

    const std::string new_arg_name = internal::getArgumentNameWithNoLeadingDashes(arg_name);

    std::map<std::string, std::string>& parsed_args = internal::getParsedArgs();

    if (parsed_args.find(new_arg_name) == parsed_args.end())
    {
        return default_value;
    }

    return getValue<T>(new_arg_name);
}

template <> std::string getValue(const std::string& arg_name, const std::string& default_value)
{
    if (arg_name.empty())
    {
        throw std::runtime_error("Argument name cannot be empty");
    }

    std::map<std::string, std::string>& parsed_args = internal::getParsedArgs();
    const std::string new_arg_name = internal::getArgumentNameWithNoLeadingDashes(arg_name);

    if (parsed_args.find(new_arg_name) == parsed_args.end())
    {
        return default_value;
    }

    return getValue<std::string>(new_arg_name);
}

template <> bool getValue(const std::string& arg_name, const bool& default_value)
{
    if (arg_name.empty())
    {
        throw std::runtime_error("Argument name cannot be empty");
    }

    const std::string new_arg_name = internal::getArgumentNameWithNoLeadingDashes(arg_name);

    std::map<std::string, std::string>& parsed_args = internal::getParsedArgs();

    if (parsed_args.find(new_arg_name) == parsed_args.end())
    {
        return default_value;
    }

    return getValue<bool>(new_arg_name);
}

}  // namespace debug_value_args

#endif  // DEMOS_ARGS_H