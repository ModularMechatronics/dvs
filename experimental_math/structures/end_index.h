#ifndef DVS_MATH_STRUCTURES_END_INDEX_H_
#define DVS_MATH_STRUCTURES_END_INDEX_H_

#include <cassert>

namespace dvs
{
struct EndIndex
{
    const int offset;

    explicit EndIndex() : offset(0) {}
    explicit EndIndex(const int offset_) : offset(offset_)
    {
        assert(offset_ <= 0 && "Offset must be either 0 or negative!");
    }
};

inline EndIndex operator-(const EndIndex& end_idx, int offset)
{
    return EndIndex(end_idx.offset - offset);
}

struct AllIndices
{
    explicit AllIndices() = default;
};

#define End EndIndex()
#define All AllIndices()

}  // namespace dvs

#endif // DVS_MATH_STRUCTURES_END_INDEX_H_
