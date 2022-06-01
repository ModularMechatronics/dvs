#include <algorithm>
#include <stdexcept>

#include "core/arg_parser.h"

#include "core/logging.h"

ArgParser::ArgParser(const ArgumentOptions& options, const int argc, const char* argv[])
{
    options_ = options;
    executable_path_ = argv[0];

    const std::vector<std::string> args_vec = createArgsVec(argc, argv);
    validateInputArgumentsBasicForm(args_vec);
    validateRequiredArgumentsPresent(args_vec);
    validateNoExtraArguments(args_vec);
    validateInputArgumentsCorrectForm(args_vec);
    parseArguments(args_vec);
}

bool ArgParser::hasFlag(const std::string& flag_arg_name) const
{
    return std::find(flag_arguments_.begin(), flag_arguments_.end(), flag_arg_name) != flag_arguments_.end();
}

void ArgParser::parseArguments(const std::vector<std::string>& args)
{
    for(size_t i = 0; i < args.size(); i++)
    {
        const std::string arg_no_leading_dash = args[i].substr(2, std::string::npos);
        std::string stripped_arg;

        const bool has_equality_sign = arg_no_leading_dash.find("=") != std::string::npos;

        if(has_equality_sign)
        {
            stripped_arg = arg_no_leading_dash.substr(0, arg_no_leading_dash.find("="));
        }
        else
        {
            stripped_arg = arg_no_leading_dash;
        }

        const ArgumentTraits arg_traits = options_.getArgumentTraits(stripped_arg);
        if(arg_traits.type == ArgumentTraits::Type::kFlag)
        {
            flag_arguments_.push_back(stripped_arg);
        }
        else if(arg_traits.type == ArgumentTraits::Type::kValued)
        {
            valued_arguments_[stripped_arg] = Argument{stripped_arg, arg_no_leading_dash.substr(arg_no_leading_dash.find("=")+1, std::string::npos)};
        }
    }
}

Argument ArgParser::operator[](const std::string& arg_name) const
{
    if(options_.hasArgument(arg_name))
    {
        if(options_.getArgumentTraits(arg_name).necessity == OptionNecessity::kRequired)
        {
            if(valued_arguments_.count(arg_name) == 0)
            {
                throw std::runtime_error("Requested argument " + arg_name + " was not provided by the user!");
            }
        }
        else
        {
            return Argument{};
        }
    }
    else
    {
        throw std::runtime_error("Requested argument " + arg_name + " was not included in the options!");
    }

    return valued_arguments_.at(arg_name);
}

std::vector<std::string> ArgParser::createArgsVec(const int argc, const char* argv[])
{
    std::vector<std::string> args;

    for(int k = 0; k < (argc - 1); k++)
    {
        args.push_back(argv[k + 1]);
    }

    return args;
}

void ArgParser::validateInputArgumentsBasicForm(const std::vector<std::string>& args)
{
    // Validate that input arguments are on correct form
    for(int k = 0; k < args.size(); k++)
    {
        std::string const arg = args[k];
        if(arg.length() < 3)
        {
            throw std::runtime_error("Argument too short: " + arg);
        }
        else if((arg[0] != '-') || (arg[1] != '-'))
        {
            throw std::runtime_error("Argument should be provided with leading \"--\", got: " + arg);
        }
    }
}

void ArgParser::validateRequiredArgumentsPresent(const std::vector<std::string>& args)
{
    std::vector<ArgumentTraits> const argument_traits = options_.getArgumentTraitsVec();
    for(size_t k = 0; k < argument_traits.size(); k++)
    {
        bool arg_found{false};

        for(size_t i = 0; i < args.size(); i++)
        {
            const std::string arg_no_leading_dash = args[i].substr(2, std::string::npos);
            std::string stripped_arg;

            if(arg_no_leading_dash.find("=") == std::string::npos)
            {
                stripped_arg = arg_no_leading_dash;
            }
            else
            {
                stripped_arg = arg_no_leading_dash.substr(0, arg_no_leading_dash.find("="));
            }

            if(stripped_arg == argument_traits[k].name)
            {
                arg_found = true;
                break;
            }
        }
        if((!arg_found) && (argument_traits[k].necessity == OptionNecessity::kRequired))
        {
            throw std::runtime_error("Required argument \"" + argument_traits[k].name + "\" was not provided!");
        }
    }
}

void ArgParser::validateNoExtraArguments(const std::vector<std::string>& args)
{
    // Validate that no extra arguments are provided
    for(size_t i = 0; i < args.size(); i++)
    {
        const std::string arg_no_leading_dash = args[i].substr(2, std::string::npos);
        std::string stripped_arg;

        if(arg_no_leading_dash.find("=") == std::string::npos)
        {
            stripped_arg = arg_no_leading_dash;
        }
        else
        {
            stripped_arg = arg_no_leading_dash.substr(0, arg_no_leading_dash.find("="));
        }

        if(!options_.hasArgument(stripped_arg))
        {
            throw std::runtime_error("Provided argument \"" + args[i] + "\" was not defined in the argument options!");
        }
    }
}

void ArgParser::validateInputArgumentsCorrectForm(const std::vector<std::string>& args)
{
    // Validate that arguments are on the correct form
    for(size_t i = 0; i < args.size(); i++)
    {
        const std::string arg_no_leading_dash = args[i].substr(2, std::string::npos);
        std::string stripped_arg;

        const bool has_equality_sign = arg_no_leading_dash.find("=") != std::string::npos;

        if(has_equality_sign)
        {
            stripped_arg = arg_no_leading_dash.substr(0, arg_no_leading_dash.find("="));
        }
        else
        {
            stripped_arg = arg_no_leading_dash;
        }

        if(!options_.hasArgument(stripped_arg))
        {
            throw std::runtime_error("Provided argument \"" + args[i] + "\" was not defined in the argument options!");
        }

        const ArgumentTraits arg_traits = options_.getArgumentTraits(stripped_arg);
        if(has_equality_sign && (arg_traits.type == ArgumentTraits::Type::kFlag))
        {
            throw std::runtime_error("Provided argument \"" + args[i] + "\" is supplied as a valued argument, but the corresponding option is specified to be a flag argument!");
        }
        else if((!has_equality_sign) && (arg_traits.type == ArgumentTraits::Type::kValued))
        {
            throw std::runtime_error("Provided argument \"" + args[i] + "\" is supplied as a flag argument, but the corresponding option is specified to be a valued argument!");
        }
    }
}

void ArgParser::printHelp() const
{
    const std::vector<ArgumentTraits> argument_traits = options_.getArgumentTraitsVec();
    std::cout << "Usage: " << executable_path_ << " [ARGS]" << std::endl;
    for(const auto& at : argument_traits)
    {
        std::cout << at.name << ": " << at.description << std::endl;
    }
}

ArgumentTraits ArgumentOptions::getArgumentTraits(const std::string argument_name) const
{
    bool arg_found{false};
    ArgumentTraits arg_traits;

    for(size_t k = 0; k < argument_traits_.size(); k++)
    {
        if(argument_traits_[k].name == argument_name)
        {
            arg_found = true;
            arg_traits = argument_traits_[k];
        }
    }

    ASSERT(arg_found) << "Couldn't find requested argument trait from name: " << argument_name;
    return arg_traits;
}

bool ArgumentOptions::hasArgument(const std::string argument_name) const
{
    for(size_t k = 0; k < argument_traits_.size(); k++)
    {
        if(argument_traits_[k].name == argument_name)
        {
            return true;
        }
    }

    return false;
}

ArgumentOptions& ArgumentOptions::addFlagArg(const std::string& argument_name, const std::string& description)
{
    ArgumentTraits arg_traits;

    arg_traits.name = argument_name;
    arg_traits.description = description;
    arg_traits.necessity = OptionNecessity::kUnknown;
    arg_traits.type = ArgumentTraits::Type::kFlag;

    argument_traits_.push_back(arg_traits);

    return *this;
}

ArgumentOptions& ArgumentOptions::addValuedArg(const std::string& argument_name, const std::string& description, const OptionNecessity necessity)
{
    ArgumentTraits arg_traits;

    arg_traits.name = argument_name;
    arg_traits.description = description;
    arg_traits.necessity = necessity;
    arg_traits.type = ArgumentTraits::Type::kValued;

    argument_traits_.push_back(arg_traits);

    return *this;
}
