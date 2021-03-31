#ifndef DVS_H_
#define DVS_H_

#include "low_level_com.h"
#include "attributes.h"

namespace dvs
{


template <typename T>
void rtPlot(const T v, const uint8_t id)
{
    CommandType command_type = CommandType::RT_PLOT;
}

}


#endif
