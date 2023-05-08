#ifndef MODULE_APPLICATION_DYNAMIC_MODULE_H
#define MODULE_APPLICATION_DYNAMIC_MODULE_H

#include <dlfcn.h>
#include <errno.h>
#include <mach-o/dyld.h>

#include <functional>
#include <map>
#include <string>

#include "module_api.h"

typedef void (*registerCallbacksFunctionType)(std::map<std::string, GuiElementCallback>& gui_element_callbacks,
                                              std::map<std::string, TimerCallback>& timer_callbacks,
                                              const std::function<GuiElement(const std::string&)>& gui_element_getter,
                                              const std::function<Timer(const std::string&)>& timer_getter);

class DynamicModule
{
public:
    DynamicModule() = delete;
    DynamicModule(const std::string& path_to_module_library);
    ~DynamicModule();

    void registerCallbacks(std::map<std::string, GuiElementCallback>& gui_element_callbacks,
                           std::map<std::string, TimerCallback>& timer_callbacks,
                           const std::function<GuiElement(const std::string&)>& gui_element_getter,
                           const std::function<Timer(const std::string&)>& timer_getter);
    void unloadModule();

private:
    void* lib_ptr_ = nullptr;
    registerCallbacksFunctionType register_callbacks_function_ = nullptr;

    void loadModule(const std::string& path_to_module_library);
};

#endif  // MODULE_APPLICATION_DYNAMIC_MODULE_H
