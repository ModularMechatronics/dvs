#include "core/serializer/global_file.h"

#include <fstream>
#include <mutex>
#include <map>

#include "core/serializer/serializable.h"
#include "core/serializer/file.h"


namespace serializer
{
namespace global
{

namespace simple
{

namespace internal
{
std::mutex l_mtx_{};
std::map<std::string, SimpleFileWriter> l_file_map_{};
}

void createNewFile(const std::string& file_name)
{
    std::lock_guard l(internal::l_mtx_);

    SimpleFileWriter file_writer(file_name);

    // std::pair<std::string, SimpleFileWriter> p(file_name, std::move(file_writer));
    internal::l_file_map_[file_name] = std::move(file_writer);
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
