#ifndef MODULES_CORE_MATH_INDEX_SPAN_H_
#define MODULES_CORE_MATH_INDEX_SPAN_H_

#include <stdio.h>

namespace dvs
{
struct IndexSpan
{
    const size_t from;
    const size_t to;

    IndexSpan() = delete;
    IndexSpan(const size_t from_, const size_t to_) : from(from_), to(to_) {}
    bool isIn(const size_t idx) const
    {
        return (idx >= from) && (idx < to);
    }
};
}  // namespace dvs

#endif
