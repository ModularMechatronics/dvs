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

    bool is_saved_;

public:
    SaveManager() = delete;
    SaveManager(const std::string& file_path)
    {
        file_path_ = file_path;
        project_file_ = ProjectFile(file_path);
        is_saved_ = true;
    }

    void setIsModified()
    {
        is_saved_ = false;
    }

    bool isSaved() const
    {
        return is_saved_;
    }

    std::string getCurrentFileName() const
    {
        size_t last = 0;
        size_t next = 0;
        while ((next = file_path_.find("/", last)) != std::string::npos)
        {
            last = next + 1;
        }

        return file_path_.substr(last);
    }

    void save(const ProjectFile& changed_project_file)
    {
        if(changed_project_file != project_file_)
        {
            const nlohmann::json j_to_save = changed_project_file.toJson();

            std::ofstream output_file(file_path_);
            output_file << std::setw(4) << j_to_save << std::endl;

            project_file_ = changed_project_file;

            is_saved_ = true;
        }
    }

    std::string getCurrentFilePath() const
    {
        return file_path_;
    }

    void openExistingFile(const std::string& file_path)
    {
        file_path_ = file_path;
        project_file_ = ProjectFile(file_path);
    }

    ProjectFile getCurrentProjectFile() const
    {
        return project_file_;
    }

};


#endif
