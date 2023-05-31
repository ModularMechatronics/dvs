#ifndef SYSTEM_TEST_ADVANCED_CPP_TESTS_H
#define SYSTEM_TEST_ADVANCED_CPP_TESTS_H

#include "advanced_cpp/append_properties.h"
#include "advanced_cpp/dynamic_plotting.h"
#include "advanced_cpp/object_transform.h"
#include "advanced_cpp/plot2.h"
#include "advanced_cpp/plot3.h"
#include "advanced_cpp/scatter2.h"
#include "advanced_cpp/scatter3.h"
#include "advanced_cpp/surf.h"
#include "advanced_cpp/tests.h"
#include "advanced_cpp/updateable_plotting.h"
#include "utils.h"

namespace advanced_cpp
{
void addTests()
{
    plot2::addTests();
    plot3_ns::addTests();
    scatter2::addTests();
    scatter3_ns::addTests();
    surf_ns::addTests();
    dynamic_plotting::addTests();
    updateable_plotting::addTests();
    object_transform::addTests();
    append_properties::addTests();
}

}  // namespace advanced_cpp

#endif  // SYSTEM_TEST_ADVANCED_CPP_TESTS_H
