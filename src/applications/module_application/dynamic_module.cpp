#include "dynamic_module.h"

void DynamicModule::loadModule(const std::string& path_to_module_library)
{
    lib_ptr_ = dlopen(path_to_module_library.c_str(), RTLD_NOW);

    if ((!lib_ptr_) || (lib_ptr_ == NULL) || (lib_ptr_ == 0) || (lib_ptr_ == nullptr))
    {
        DVS_LOG_ERROR() << "Library not loaded!";
        return;
    }

    char* error_str = dlerror();

    if (error_str != NULL)
    {
        DVS_LOG_ERROR() << "Error: " << error_str;
        return;
    }

    register_callbacks_function_ =
        reinterpret_cast<registerCallbacksFunctionType>(dlsym(lib_ptr_, "registerCallbacks"));

    if ((!register_callbacks_function_) || (register_callbacks_function_ == NULL) ||
        (register_callbacks_function_ == 0) || (register_callbacks_function_ == nullptr))
    {
        DVS_LOG_ERROR() << "Function not loaded!";
    }

    error_str = dlerror();

    if (error_str != NULL)
    {
        register_callbacks_function_ = nullptr;
        DVS_LOG_ERROR() << "Error: " << error_str;
        return;
    }
}

DynamicModule::DynamicModule(const std::string& path_to_module_library)
    : lib_ptr_{nullptr}, register_callbacks_function_{nullptr}
{
    loadModule(path_to_module_library);
}

DynamicModule::~DynamicModule()
{
    unloadModule();
}

void DynamicModule::unloadModule()
{
    dlclose(lib_ptr_);
}

void DynamicModule::registerCallbacks(std::map<std::string, GuiElementCallback>& callbacks,
                                      const std::function<GuiElement*(const std::string&)>& gui_element_getter)
{
    if (register_callbacks_function_ != nullptr)
    {
        register_callbacks_function_(callbacks, gui_element_getter);
    }
    else
    {
        DVS_LOG_ERROR() << "No valid function to register callbacks!";
    }
}
