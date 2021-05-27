#ifndef TEST_APP_TEST_UTILS_H_
#define TEST_APP_TEST_UTILS_H_

#include <stdlib.h>
#include <unistd.h>

inline void sleepMS(const int ms)
{
    usleep(ms * 1000);
}


#endif
