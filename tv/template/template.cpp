#include "template.h"

template <typename T>
T templatedFunction(T t0, T t1)
{
    return t0 + t1;
}

template double templatedFunction(double t0, double t1);
template int templatedFunction(int t0, int t1);
