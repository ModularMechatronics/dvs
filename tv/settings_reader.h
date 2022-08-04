#ifndef DVS_SETTINGS_READER_H_
#define DVS_SETTINGS_READER_H_

#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

#include <unistd.h>

#include "filesystem_include.h"
#include "dvs/logging.h"

struct AppPreferences
{
    std::string last_opened_file;
    bool open_main_window_on_start;
};

constexpr char* kConfigFolderName = ".config";

class ConfigurationAgent
{
public:
    ~ConfigurationAgent() {}

    ConfigurationAgent() : ConfigurationAgent{kConfigFolderName}
    {
    }

    ConfigurationAgent(const std::string& configuration_folder_name) : is_valid_{true}
    {
        const std::pair<std::string, bool> username_and_status{getUsername()};

        if(!username_and_status.second)
        {
            DVS_LOG_ERROR() << "Failed to get login name! Exiting...";
            is_valid_ = false;
            return;
        }

        const dvs_filesystem::path home_dir_path("/home/" + username_and_status.first);
        const dvs_filesystem::path config_dir_path{home_dir_path / dvs_filesystem::path{configuration_folder_name}};
        const dvs_filesystem::path dvs_dir_path{config_dir_path / dvs_filesystem::path{"dvs"}};

        if (!dvs_filesystem::exists(config_dir_path))
        {
            DVS_LOG_INFO() << config_dir_path << " dir does not exist, creating...";
            try
            {
                dvs_filesystem::create_directory(config_dir_path);
            }
            catch(const std::exception& e)
            {
                DVS_LOG_ERROR() << "Failed to create config directory at \"" << config_dir_path << "\". Exception: " <<  e.what();
                is_valid_ = false;
                return;
            }
        }

        if (!dvs_filesystem::exists(dvs_dir_path))
        {
            DVS_LOG_INFO() << "dvs dir does not exist, creating...";
            try
            {
                dvs_filesystem::create_directory(dvs_dir_path);
            }
            catch(const std::exception& e)
            {
                DVS_LOG_ERROR() << "Failed to create dvs directory at \"" << dvs_dir_path << "\". Exception: " <<  e.what();
                is_valid_ = false;
                return;
            }
        }
        configuration_file_path_ = dvs_filesystem::path(dvs_dir_path / "configuration.json");

        if (!dvs_filesystem::exists(configuration_file_path_))
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

    // TODO: Move to a more general location
    static std::pair<std::string, bool> getUsername()
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
    }

    bool isValid() const
    {
        return is_valid_;
    }

    bool hasKey(const std::string& key)
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

    template <typename T> T readValue(const std::string& key)
    {
        if(is_valid_)
        {
            nlohmann::json json_data;
            T data;
            try
            {
                json_data = readConfigurationFile();
            }
            catch(const std::exception& e)
            {
                is_valid_ = false;
                DVS_LOG_ERROR() << "Error reading configuration file \"" << configuration_file_path_ << "\"! Returning empty value.";
                return data;
            }

            try
            {
                data = json_data[key].get<T>();
            }
            catch(const std::exception& e)
            {
                DVS_LOG_ERROR() << "Error reading value for key " << key << "! Returning empty value.";
            }
            return data;
        }
        else
        {
            DVS_LOG_ERROR() << "Tried calling ConfigurationAgent::readValue(" << key << ") for invalid ConfigurationAgent!";
            T data;
            return data;
        }
    }

    template <typename T> void writeValue(const std::string& key, const T& val)
    {
        if(is_valid_)
        {
            nlohmann::json json_data = readConfigurationFile();
            json_data[key] = val;

            try
            {
                writeToConfigurationFile(json_data);
            }
            catch(const std::exception& e)
            {
                DVS_LOG_ERROR() << "Failed to overwrite configuration.json at \"" << configuration_file_path_ << "\". Exception: " <<  e.what() << ". Exiting.";
                is_valid_ = false;
                return;
            }
        }
        else
        {
            DVS_LOG_ERROR() << "Tried calling ConfigurationAgent::writeValue(" << key << ") for invalid ConfigurationAgent!";
        }
    }

private:

    nlohmann::json readConfigurationFile() const
    {
        std::ifstream input_file(configuration_file_path_);
        nlohmann::json json_data;
        input_file >> json_data;
        return json_data;
    }

    void createEmptyConfigurationFile() const
    {
        std::ofstream output_file_stream(configuration_file_path_);
        output_file_stream << "{\n}\n";
    }

    void writeToConfigurationFile(const nlohmann::json& json_data) const
    {
        std::ofstream output_file(configuration_file_path_);
        output_file << std::setw(4) << json_data << std::endl;
    }

    bool is_valid_;
    std::string configuration_file_path_;
};

#endif
