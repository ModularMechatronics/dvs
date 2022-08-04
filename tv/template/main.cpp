#include <time.h>
#include <cstdlib>
#include <stdint.h>

#include <vector>
#include <iostream>
#include <utility>
#include <chrono>
#include <cstring>
#include <functional>
#include <type_traits>

#include "template.h"

void aFunction(float* const f)
{
    std::cout << f[0] << std::endl;
    f[0] = 2;
}

int main()
{
    /*int i0 = 2, i1 = 5;
    double d0 = 2, d1 = 5;
    float f0 = 2, f1 = 5;
    std::cout << templatedFunction(d0, d1) << std::endl;
    std::cout << templatedFunction(i0, i1) << std::endl;
    std::cout << templatedFunction(f0, f1) << std::endl;*/

    float  * const a_ptr = new float[2];
    aFunction(a_ptr);
    // a_ptr[0] = 1;


}
