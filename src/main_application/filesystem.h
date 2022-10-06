#ifndef MAIN_APPLICATION_FILESYSTEM_INCLUDE_H_
#define MAIN_APPLICATION_FILESYSTEM_INCLUDE_H_

#ifdef PLATFORM_LINUX_M
#include <experimental/filesystem>
namespace dvs
{
namespace filesystem = std::experimental::filesystem;
}
#endif

#ifdef PLATFORM_APPLE_M
#include <filesystem>
namespace dvs
{
namespace filesystem = std::filesystem;
}
#endif

#endif  // MAIN_APPLICATION_FILESYSTEM_INCLUDE_H_
