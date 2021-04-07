#ifndef DVS_END_INDEX_H_
#define DVS_END_INDEX_H_

#include <cassert>

namespace dvs
{
struct EndIndex
{
    const int offset;

    EndIndex() : offset(0) {}
    EndIndex(const int offset_) : offset(offset_)
    {
        assert(offset_ <= 0 && "Offset must be either 0 or negative!");
    }
};

}  // namespace dvs

#endif
