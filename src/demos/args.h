#ifndef DEMOS_ARGS_H
#define DEMOS_ARGS_H

#include <cstdint>
#include <map>
#include <string>
#include <type_traits>

extern std::map<std::string, std::vector<std::uint8_t>> parsed_args;

inline void parseArgs(int argc, char* argv[])
{
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
                    parsed_args[arg_name] = std::vector<std::uint8_t>(arg_value.begin(), arg_value.end());
                }
                else
                {
                    throw std::runtime_error("No valid value following argument " + arg_name + "!");
                }
            }
            else
            {
                throw std::runtime_error("Argument " + arg_name + " has no value");
            }
        }
    }
}

template <typename T> T getArg(const std::string& arg_name)
{
    if (parsed_args.find(arg_name) == parsed_args.end())
    {
        throw std::runtime_error("Argument " + arg_name + " not found");
    }

    if (std::is_same<T, float>::value)
    {
        return std::stof(std::string(parsed_args[arg_name].begin(), parsed_args[arg_name].end()));
    }
    else if (std::is_same<T, double>::value)
    {
        return std::stod(std::string(parsed_args[arg_name].begin(), parsed_args[arg_name].end()));
    }
}

template <typename T> T getArg(const std::string& arg_name, const T default_value)
{
    if (parsed_args.find(arg_name) == parsed_args.end())
    {
        return default_value;
    }

    return getArg<T>(arg_name);
}

#endif  // DEMOS_ARGS_H