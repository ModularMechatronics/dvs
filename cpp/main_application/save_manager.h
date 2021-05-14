#ifndef SAVE_MANAGER_H_
#define SAVE_MANAGER_H_

#include <string>

#include <nlohmann/json.hpp>

#include "project_file.h"

class SaveManager
{
private:
    std::string file_path_;
    ProjectFile project_file_;

public:
    SaveManager() = delete;
    SaveManager(const std::string& file_path)
    {
        file_path_ = file_path;
        project_file_ = ProjectFile(file_path);
    }

    void save(const ProjectFile& changed_project_file)
    {
        if(changed_project_file != project_file_)
        {
            const nlohmann::json j_to_save = changed_project_file.toJson();

            std::ofstream output_file(file_path_);
            output_file << std::setw(4) << j_to_save << std::endl;

            project_file_ = changed_project_file;
            std::cout << "Saved settings!" << std::endl;
        }
        else
        {
            std::cout << "Nothing has changed!" << std::endl;
        }
    }

    ProjectFile getCurrentProjectFile() const
    {
        return project_file_;
    }

};


#endif
