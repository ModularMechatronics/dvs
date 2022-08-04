#ifndef MODULES_CORE_ARG_PARSER_ARG_PARSER_H_
#define MODULES_CORE_ARG_PARSER_ARG_PARSER_H_

#include <vector>
#include <string>
#include <stdexcept>
#include <map>
#include <type_traits>

// constexpr char kAnOption[] = "aaaa";

class Argument
{
private:
    std::string arg_name_;
    std::string data_;
    bool has_data_;

public:
    Argument() : arg_name_{""}, data_{""}, has_data_{false}
    {

    }

    Argument(const std::string& arg_name, const std::string& data) : arg_name_{arg_name}, data_{data}, has_data_{true} {}

    template<typename T>
    typename std::enable_if<std::is_same<T, std::string>::value, T>::type valueOr(const std::string& default_value)
    {
        if(has_data_)
        {
            return data_;
        }
        else
        {
            return default_value;
        }
    }

    template<typename T>
    typename std::enable_if<std::is_same<T, float>::value, T>::type valueOr(const float& default_value)
    {
        if(has_data_)
        {
            return std::stof(data_);
        }
        else
        {
            return default_value;
        }
    }

    template<typename T>
    typename std::enable_if<std::is_same<T, int>::value, T>::type valueOr(const int& default_value)
    {
        if(has_data_)
        {
            return std::stoi(data_);
        }
        else
        {
            return default_value;
        }
    }

    template<typename T>
    typename std::enable_if<std::is_same<T, bool>::value, T>::type valueOr(const bool& default_value)
    {
        if(has_data_)
        {
            if(data_ == "true")
            {
                return true;
            }
            else if(data_ == "true")
            {
                return false;
            }
            else
            {
                throw std::runtime_error("Requested bool for argument " + arg_name_ + " had invalid value \"" + data_ + "\"! Choose between valid values {true,false}.");
                return false;
            }
        }
        else
        {
            return default_value;
        }
    }

    bool hasValue() const
    {
        return has_data_;
    }

};

enum class OptionNecessity
{
    kUnknown,
    kRequired,
    kOptional
};

struct ArgumentTraits
{
    enum class Type
    {
        kUnknown,
        kFlag,
        kValued
    };

    std::string name;
    std::string description;
    Type type;
    OptionNecessity necessity;
};

class ArgumentOptions
{
private:
    std::vector<ArgumentTraits> argument_traits_;

public:
    bool hasArgument(const std::string argument_name) const;
    ArgumentTraits getArgumentTraits(const std::string argument_name) const;

    ArgumentOptions& addValuedArg(const std::string& argument_name, const std::string& description, const OptionNecessity necessity = OptionNecessity::kRequired);
    ArgumentOptions& addFlagArg(const std::string& argument_name, const std::string& description);

    std::vector<ArgumentTraits> getArgumentTraitsVec() const
    {
        return argument_traits_;
    }
};

class ArgParser
{
private:
    ArgumentOptions options_;
    std::string executable_path_;
    std::vector<std::string> flag_arguments_;
    std::map<std::string, Argument> valued_arguments_;

    std::vector<std::string> createArgsVec(const int argc, const char* argv[]);
    void validateInputArgumentsBasicForm(const std::vector<std::string>& args);
    void validateRequiredArgumentsPresent(const std::vector<std::string>& args);
    void validateNoExtraArguments(const std::vector<std::string>& args);
    void validateInputArgumentsCorrectForm(const std::vector<std::string>& args);
    void parseArguments(const std::vector<std::string>& args);


public:
    ArgParser() = default;
    ArgParser(const ArgumentOptions& options, const int argc, const char* argv[]);

    void printHelp() const;

    Argument operator[](const std::string& arg_name) const;
    bool hasFlag(const std::string& flag_arg_name) const;

    void appendValuedArgument(const std::string& arg_name, const std::string& arg_value)
    {
        valued_arguments_[arg_name] = Argument{arg_name, arg_value};
    }

    void appendFlagArgument(const std::string& arg_name)
    {
        flag_arguments_.push_back(arg_name);
    }

};

#endif
