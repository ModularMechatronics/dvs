#ifndef MAIN_APPLICATION_FILESYSTEM_INCLUDE_H_
#define MAIN_APPLICATION_FILESYSTEM_INCLUDE_H_

// TODO: Rename file to "filesystem.h" and put "dvs_filesystem" into dvs/omniplot namespace and just call it "filesystem"

#ifdef PLATFORM_LINUX_M
#include <experimental/filesystem>
namespace dvs_filesystem = std::experimental::filesystem;
#endif

#ifdef PLATFORM_APPLE_M
#include <filesystem>
namespace dvs_filesystem = std::filesystem;
#endif

#endif // MAIN_APPLICATION_FILESYSTEM_INCLUDE_H_
