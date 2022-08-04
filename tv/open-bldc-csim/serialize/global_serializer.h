#ifndef SERIALIZABLE_GLOBAL_SERIALIZER_H_
#define SERIALIZABLE_GLOBAL_SERIALIZER_H_

#include <fstream>
#include <mutex>
#include <map>

#include <stdint.h>
#include "serializable.h"
#include "file.h"

namespace serializer
{


namespace internal
{
inline std::mutex& getMutex()
{
    static std::mutex mtx;
    return mtx;
}

inline std::map<std::string, SimpleFileWriter>& getFileMap()
{
    static std::map<std::string, SimpleFileWriter> file_map;
    return file_map;
}

}

namespace global
{

namespace simple
{

inline void createNewFile(const std::string& file_name)
{
    std::lock_guard l(internal::getMutex());
    std::map<std::string, SimpleFileWriter>& file_map = internal::getFileMap();

    SimpleFileWriter file_writer(file_name);

    // std::pair<std::string, SimpleFileWriter> p(file_name, std::move(file_writer));
    file_map[file_name] = std::move(file_writer);
    // file_map.insert(std::move(p));
    // file_map.emplace(file_name, std::move(file_writer));
    // file_map[file_name].open(file_name);

    std::cout << "Here" << std::endl;

    // file_map.insert(std::make_pair(file_name, std::move(file_writer)));
    // file_map.insert(file_name, std::move(file_writer));

    // std::pair<std::string, SimpleFileWriter> p(file_name, std::move(SimpleFileWriter(file_name)));
    // file_map.insert(std::move(p));
    // file_map.emplace(std::make_pair<std::string, SimpleFileWriter>(file_name, file_name)));

    // std::string tmp_str = file_name;
    // file_map.emplace(tmp_str, file_name);

}

}

}



}

#endif
