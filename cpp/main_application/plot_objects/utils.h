#ifndef PLOT_OBJECTS_UTILS_H_
#define PLOT_OBJECTS_UTILS_H_

#include <iostream>
#include <stdint.h>

#include "enumerations.h"

using namespace dvs::internal;

size_t getNumDimensionsFromFunction(const Function fcn)
{

    switch(fcn)
    {
        case Function::PLOT2:
            return 2;
        case Function::PLOT3:
            return 3;
        case Function::SCATTER2:
            return 2;
        case Function::SCATTER3:
            return 3;
        case Function::SURF:
            return 3;
        default:
            std::cout << "You haven't defined a case for Function type " << static_cast<int>(fcn) << std::endl;
            exit(-1);
            return -1;
    }
}

#endif
