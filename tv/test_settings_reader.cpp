#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <dirent.h>

#include "main_application/project_state/settings_reader.h"

std::string getConfigFilePath(const std::string& config_folder_name);

constexpr char* kConfigFolderNameLocal = ".config2";
const std::string kConfigFilePath{getConfigFilePath(kConfigFolderNameLocal)};

std::string getConfigFilePath(const std::string& config_folder_name)
{
    const auto username_and_status = ConfigurationAgent::getUsername();
    if(!username_and_status.second)
    {
        std::cout << "Failed to get username!" << std::endl;
        exit(-1);
    }

    const std::string config_file_path{"/home/" + username_and_status.first + "/" + config_folder_name + "/dvs/" + "configuration.json"};
    return config_file_path;
}

void removeFile(const std::string& config_file_path)
{
    system(("rm -f " + config_file_path).c_str());
}

TEST(TestConfigurationAgent, TestBasic)
{
    removeFile(kConfigFilePath);
    ASSERT_FALSE(dvs_filesystem::exists(kConfigFilePath));

    const ConfigurationAgent configuration_agent{kConfigFolderNameLocal};
    ASSERT_TRUE(dvs_filesystem::exists(kConfigFilePath));

    ASSERT_TRUE(configuration_agent.isValid());
}

TEST(TestConfigurationAgent, TestPersistentFile)
{
    removeFile(kConfigFilePath);
    ASSERT_FALSE(dvs_filesystem::exists(kConfigFilePath));

    ConfigurationAgent configuration_agent{kConfigFolderNameLocal};
    ASSERT_TRUE(dvs_filesystem::exists(kConfigFilePath));

    const std::string key{"test_key"};
    const std::string val{"test_val"};
    configuration_agent.writeValue(key, val);

    ConfigurationAgent configuration_agent_new{kConfigFolderNameLocal};

    ASSERT_TRUE(configuration_agent_new.hasKey(key));
    ASSERT_EQ(configuration_agent_new.readValue<std::string>(key), val);
}

TEST(TestConfigurationAgent, TestReadWrite)
{
    removeFile(kConfigFilePath);
    ASSERT_FALSE(dvs_filesystem::exists(kConfigFilePath));

    ConfigurationAgent configuration_agent{kConfigFolderNameLocal};
    ASSERT_TRUE(dvs_filesystem::exists(kConfigFilePath));

    for(size_t k = 0; k < 10; k++)
    {
        const std::string key{"test_key" + std::to_string(k)};
        const std::string val{"test_val" + std::to_string(k)};

        configuration_agent.writeValue(key, val);
    }

    for(size_t k = 0; k < 10; k++)
    {
        const std::string key{"test_key" + std::to_string(k)};
        const std::string val{"test_val" + std::to_string(k)};

        configuration_agent.writeValue(key, val);
    }

    std::ifstream input_file(kConfigFilePath);
    nlohmann::json json_data;
    input_file >> json_data;

    for(size_t k = 0; k < 10; k++)
    {
        const std::string key{"test_key" + std::to_string(k)};
        const std::string val{"test_val" + std::to_string(k)};

        ASSERT_EQ(json_data[key], val);
    }
}

TEST(TestConfigurationAgent, TestEmptyFile)
{
    removeFile(kConfigFilePath);
    ASSERT_FALSE(dvs_filesystem::exists(kConfigFilePath));

    ConfigurationAgent configuration_agent{kConfigFolderNameLocal};
    ASSERT_TRUE(dvs_filesystem::exists(kConfigFilePath));

    std::ifstream input_file(kConfigFilePath);
    std::stringstream read_buffer;

    read_buffer << input_file.rdbuf();

    ASSERT_EQ(read_buffer.str(),"{\n}\n");
}

TEST(TestConfigurationAgent, TestCorruptFile)
{
    removeFile(kConfigFilePath);
    ASSERT_FALSE(dvs_filesystem::exists(kConfigFilePath));

    std::ofstream output_file_stream(kConfigFilePath);
    output_file_stream << "{\nGarbageData}\n";
    ASSERT_TRUE(dvs_filesystem::exists(kConfigFilePath));

    const ConfigurationAgent configuration_agent{kConfigFolderNameLocal};
    ASSERT_TRUE(dvs_filesystem::exists(kConfigFilePath));
    ASSERT_TRUE(configuration_agent.isValid());

    std::ifstream input_file(kConfigFilePath);
    std::stringstream read_buffer;

    read_buffer << input_file.rdbuf();

    ASSERT_EQ(read_buffer.str(),"{\n}\n");
}
