#ifndef SERIALIZABLE_GLOBAL_SERIALIZER_H_
#define SERIALIZABLE_GLOBAL_SERIALIZER_H_

#include <stdint.h>

#include <string>
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
extern std::mutex l_mtx_;
extern std::map<std::string, SimpleFileWriter> l_file_map_;

}

void createNewFile(const std::string& file_name);

template <typename T>
void append(const std::string& file_name, const T& value)
{
    std::lock_guard l(internal::l_mtx_);

    SimpleFileWriter& file = internal::l_file_map_[file_name];
    // file.appendStaticType(value);
}



}

}



}

#endif
