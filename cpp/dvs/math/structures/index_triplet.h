#ifndef DVS_INDEX_TRIPLET_H_
#define DVS_INDEX_TRIPLET_H_

#include <stdint.h>

namespace dvs
{

struct IndexTriplet
{
    uint32_t i0;
    uint32_t i1;
    uint32_t i2;

    IndexTriplet() = default;
    IndexTriplet(const uint32_t i0_, const uint32_t i1_, const uint32_t i2_)
    {
        i0 = i0_;
        i1 = i1_;
        i2 = i2_;
    }

};

}

#endif
