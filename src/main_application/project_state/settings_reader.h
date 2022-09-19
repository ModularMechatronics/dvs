#ifndef MAIN_APPLICATION_PROJECT_STATE_SETTINGS_READER_H_
#define MAIN_APPLICATION_PROJECT_STATE_SETTINGS_READER_H_

#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>

#include "filesystem.h"

class SettingsReader
{
private:
    std::string settings_file_path_;
    nlohmann::json j_;

public:
    SettingsReader() = delete;
    ~SettingsReader()
    {
        createCacheFile();
    }
    SettingsReader(const dvs::filesystem::path& settings_file_path)
    {
        settings_file_path_ = settings_file_path;
        settings_file_path_ += dvs::filesystem::path("cache.json");
        j_["empty"] = "empty";

        if (!dvs::filesystem::exists(settings_file_path_))
        {
            std::cout << "File doesn't exist! Creating..." << std::endl;
            createCacheFile();
        }
        else
        {
            std::cout << "File exists! Reading..." << std::endl;
            try
            {
                readCacheFile();
            }
            catch (const std::exception& e)
            {
                std::cout << "Exception caught during reading cache file, creating new..." << std::endl;
                createCacheFile();
            }
        }
        std::cout << settings_file_path_ << std::endl;
    }

    bool hasKey(const std::string& key)
    {
        return j_.count(key);
    }

    template <typename T> T readCache(const std::string& key)
    {
        return j_[key];
    }

    template <typename T> void writeToCache(const std::string& key, const T& val)
    {
        j_[key] = val;
        createCacheFile();
    }

    void readCacheFile()
    {
        std::ifstream input_file(settings_file_path_);
        input_file >> j_;
    }

    void createCacheFile()
    {
        std::ofstream output_file(settings_file_path_);
        output_file << std::setw(4) << j_ << std::endl;
    }
};

#endif // MAIN_APPLICATION_PROJECT_STATE_SETTINGS_READER_H_
