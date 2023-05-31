#ifndef SYSTEM_TEST_UTILS_H
#define SYSTEM_TEST_UTILS_H

#include <functional>
#include <map>
#include <string>

using TestMap = std::map<std::string, std::map<std::string, std::map<std::string, std::function<void()>>>>;

extern TestMap tests_;

inline void addTest(const std::string& language,
                    const std::string& test_group,
                    const std::string& test_name,
                    std::function<void()> test_function)
{
    tests_[language][test_group][test_name] = test_function;
}

#endif  // SYSTEM_TEST_UTILS_H