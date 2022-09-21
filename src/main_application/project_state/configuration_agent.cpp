#include "project_state/configuration_agent.h"

#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>

#include <iomanip>

#ifdef PLATFORM_LINUX_M
#include <libgen.h>
#include <linux/limits.h>
#include <unistd.h>

#endif

#ifdef PLATFORM_APPLE_M

#include <mach-o/dyld.h>

#endif

constexpr char* kConfigFolderName = ".config";


#ifdef PLATFORM_LINUX_M
std::string getExecutablePath()
{
    std::array<char, PATH_MAX> result;
    ssize_t count = readlink("/proc/self/exe", result.data(), PATH_MAX);
    const char* path;
    if (count != -1)
    {
        path = dirname(result.data());
    }
    return std::string(path);
}

#endif

#ifdef PLATFORM_APPLE_M

std::string getExecutablePath()
{
    std::array<char, 2048> path;
    uint32_t size = path.size();

    if (_NSGetExecutablePath(path.data(), &size) != 0)
    {
        printf("Buffer too small; need size %u\n", size);
    }

    return std::string(path.data());
}

#endif

// TODO: Move to a more general location
/*static std::pair<std::string, bool> getUsername()
{
    char username_buffer[256];

    if(getlogin_r(username_buffer, 256) == 0)
    {
        return std::make_pair<>(std::string{username_buffer}, true);
    }
    else
    {
        return std::make_pair<>("", false);
    }
}*/

static std::pair<std::string, bool> getHomeDirPath()
{
    const char* home_dir = getenv("HOME");

    if (!home_dir)
    {
        const struct passwd* const pwd = getpwuid(getuid());
        if (pwd)
        {
            home_dir = pwd->pw_dir;
        }
        else
        {
            return std::make_pair<>("", false);
        }
    }

    return std::make_pair<>(home_dir, true);
}

ConfigurationAgent::~ConfigurationAgent() {}

// TODO: Implement fallback of execution dir if home dir is not available
// dvs::filesystem::path pa = dvs::filesystem::absolute(getExecutablePath());
// configuration_agent_ = new ConfigurationAgent(pa.remove_filename());

ConfigurationAgent::ConfigurationAgent() : ConfigurationAgent{kConfigFolderName}
{
}

ConfigurationAgent::ConfigurationAgent(const std::string& configuration_folder_name) : is_valid_{true}
{
#ifdef PLATFORM_LINUX_M
    const std::pair<std::string, bool> username_and_status{getUsername()};

    if(!username_and_status.second)
    {
        DVS_LOG_ERROR() << "Failed to get login name! Exiting...";
        is_valid_ = false;
        return;
    }
    const dvs::filesystem::path home_dir_path("/home/" + username_and_status.first);
    const dvs::filesystem::path config_dir_path{home_dir_path / dvs::filesystem::path{configuration_folder_name}};
#endif

#ifdef PLATFORM_APPLE_M

    const std::pair<std::string, bool> home_dir_path_and_status{getHomeDirPath()};

    if(!home_dir_path_and_status.second)
    {
        DVS_LOG_ERROR() << "Failed to get home dir path! Exiting...";
        is_valid_ = false;
        return;
    }

    const dvs::filesystem::path home_dir_path(home_dir_path_and_status.first);
    const dvs::filesystem::path config_dir_path{home_dir_path / dvs::filesystem::path{configuration_folder_name}};
#endif

#ifdef PLATFORM_WINDOWS_M
    const dvs::filesystem::path config_dir_path{home_dir_path / dvs::filesystem::path{configuration_folder_name}};
#endif

    const dvs::filesystem::path dvs_dir_path{config_dir_path / dvs::filesystem::path{"dvs"}};

    if (!dvs::filesystem::exists(config_dir_path))
    {
        DVS_LOG_INFO() << config_dir_path << " dir does not exist, creating...";
        try
        {
            dvs::filesystem::create_directory(config_dir_path);
        }
        catch(const std::exception& e)
        {
            DVS_LOG_ERROR() << "Failed to create config directory at \"" << config_dir_path << "\". Exception: " <<  e.what();
            is_valid_ = false;
            return;
        }
    }

    if (!dvs::filesystem::exists(dvs_dir_path))
    {
        DVS_LOG_INFO() << "dvs dir does not exist, creating...";
        try
        {
            dvs::filesystem::create_directory(dvs_dir_path);
        }
        catch(const std::exception& e)
        {
            DVS_LOG_ERROR() << "Failed to create dvs directory at \"" << dvs_dir_path << "\". Exception: " <<  e.what();
            is_valid_ = false;
            return;
        }
    }
    configuration_file_path_ = dvs::filesystem::path(dvs_dir_path / "configuration.json");

    if (!dvs::filesystem::exists(configuration_file_path_))
    {
        DVS_LOG_INFO() << "The file \"" << configuration_file_path_ << "\" file does not exist, creating...";
        try
        {
            createEmptyConfigurationFile();
        }
        catch(const std::exception& e)
        {
            DVS_LOG_ERROR() << "Failed to create configuration file at \"" << configuration_file_path_ << "\". Exception: " <<  e.what();
            is_valid_ = false;
            return;
        }
    }
    else
    {
        bool reading_successful{false};
        try
        {
            readConfigurationFile();
            reading_successful = true;
        }
        catch(const std::exception& e)
        {
            DVS_LOG_WARNING() << "Error reading existing configuration file \"" << configuration_file_path_ << "\". Exception: " <<  e.what() << ". Creating a new one.";
        }

        if(!reading_successful)
        {
            try
            {
                createEmptyConfigurationFile();
            }
            catch(const std::exception& e)
            {
                DVS_LOG_ERROR() << "Failed to create configuration.json at \"" << configuration_file_path_ << "\". Exception: " <<  e.what() << ". Exiting.";
                is_valid_ = false;
                return;
            }
        }
    }
}

bool ConfigurationAgent::isValid() const
{
    return is_valid_;
}

bool ConfigurationAgent::hasKey(const std::string& key)
{
    if(is_valid_)
    {
        try
        {
            const nlohmann::json json_data = readConfigurationFile();
            return json_data.count(key) > 0;
        }
        catch(const std::exception& e)
        {
            DVS_LOG_ERROR() << "Error calling ConfigurationAgent::hasKey(" << key << ") in configuration file \"" << configuration_file_path_ << "\".";
            return 0;
        }
    }
    else
    {
        DVS_LOG_ERROR() << "Tried calling ConfigurationAgent::hasKey(" << key << ") for invalid ConfigurationAgent!";
        return false;
    }
}

nlohmann::json ConfigurationAgent::readConfigurationFile() const
{
    std::ifstream input_file(configuration_file_path_);
    nlohmann::json json_data;
    input_file >> json_data;
    return json_data;
}

void ConfigurationAgent::createEmptyConfigurationFile() const
{
    std::ofstream output_file_stream(configuration_file_path_);
    output_file_stream << "{\n}\n";
    std::cout << "Creating file at path: " << configuration_file_path_ << std::endl;
}

void ConfigurationAgent::writeToConfigurationFile(const nlohmann::json& json_data) const
{
    std::ofstream output_file(configuration_file_path_);
    output_file << std::setw(4) << json_data << std::endl;
}
