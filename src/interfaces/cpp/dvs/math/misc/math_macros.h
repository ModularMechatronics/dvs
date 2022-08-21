#ifndef DVS_MATH_MISC_MATH_MACROS_H_
#define DVS_MATH_MISC_MATH_MACROS_H_

#include "dvs/logging.h"

namespace dvs
{
#define DATA_ALLOCATION(data, data_size, data_type, alloc_type)                                   \
    try                                                                                           \
    {                                                                                             \
        data = new data_type[data_size];                                                          \
    }                                                                                             \
    catch (std::bad_alloc & ba)                                                                   \
    {                                                                                             \
        std::cerr << alloc_type << " allocation failed, bad_alloc caught: " << ba.what() << '\n'; \
        exit(-1);                                                                                 \
    }

#define ASSERT_MAT_VALID(mat)                                 \
    DVS_ASSERT(mat.isAllocated()) << "Matrix not allocated!"; \
    DVS_ASSERT(mat.rows() > 0) << "Number of rows is 0!";     \
    DVS_ASSERT(mat.cols() > 0) << "Number of cols is 0!"

#define ASSERT_VEC_VALID(vec)                                 \
    DVS_ASSERT(vec.isAllocated()) << "Matrix not allocated!"; \
    DVS_ASSERT(vec.rows() > 0) << "Vector size is 0!";

#define ASSERT_MAT_VALID_INTERNAL()                       \
    DVS_ASSERT(is_allocated_) << "Matrix not allocated!"; \
    DVS_ASSERT(num_rows_ > 0) << "Number of rows is 0!";  \
    DVS_ASSERT(num_cols_ > 0) << "Number of cols is 0!"

#define ASSERT_VEC_VALID_INTERNAL()                       \
    DVS_ASSERT(is_allocated_) << "Vector not allocated!"; \
    DVS_ASSERT(vector_length_ > 0) << "Number of elements is 0!"

}  // namespace dvs

#endif // DVS_MATH_MISC_MATH_MACROS_H_
