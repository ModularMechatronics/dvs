#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "arl/math.h"
#include "test/unit_tests/math/math_test_utils.h"

namespace arl
{
namespace
{
class VectorIndexingTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(VectorIndexingTest, VectorIndexingLogical)
{
    const Vector<int> v0({1, 4, 3, 2, 6, 3, 1, 0, -3, 2, 5, 4});
    const Vector<bool> v_ind({1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1});

    const Vector<int> vres = v0(v_ind);

    size_t idx = 0;
    for (size_t k = 0; k < v_ind.size(); k++)
    {
        if (v_ind(k))
        {
            ASSERT_EQ(v0(k), vres(idx));
            idx++;
        }
    }
}

TEST_F(VectorIndexingTest, VectorIndexingIndices)
{
    const Vector<int> v0({1, 4, 3, 2, 6, 3, 1, 0, -3, 2, 5, 4});
    const Vector<size_t> v_ind0({0, 1, 0, 4, 2, 6, 2});
    const Vector<int> v_ind1({0, 1, 0, 4, 2, 6, 2, 1, 6, 3, 2, 4, 6, 2, 3});

    const Vector<int> vres0 = v0(v_ind0);
    const Vector<int> vres1 = v0(v_ind1);

    for (size_t k = 0; k < v_ind0.size(); k++)
    {
        const size_t idx = v_ind0(k);
        ASSERT_EQ(v0(idx), vres0(k));
    }

    for (size_t k = 0; k < v_ind1.size(); k++)
    {
        const size_t idx = v_ind1(k);
        ASSERT_EQ(v0(idx), vres1(k));
    }
}

TEST_F(VectorIndexingTest, VectorIndexingIndicesAngLogical)
{
    const Vector<int> v0({1, 4, 3, 2, 6, 3, 1, 0, -3, 2, 5, 4});
    const Vector<size_t> v_ind0({0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1});
    const Vector<size_t> v_ind1({0, 1, 2, 5, 2, 4, 2, 4, 1, 3, 4, 0});

    const Vector<int> vres0 = v0(v_ind0);
    const Vector<int> vres1 = v0(v_ind1);
    const Vector<int> v_exp0 = {4, 2, 6, 0, -3, 5, 4};

    for (size_t k = 0; k < v_exp0.size(); k++)
    {
        ASSERT_EQ(vres0(k), v_exp0(k));
    }
    ASSERT_EQ(v_exp0.size(), vres0.size());

    for (size_t k = 0; k < v_ind1.size(); k++)
    {
        const size_t idx = v_ind1(k);
        ASSERT_EQ(v0(idx), vres1(k));
    }
}

TEST_F(VectorIndexingTest, VectorEndIndexing)
{
    const Vector<int> v({0, 1, 0, 4, 2, 6, 2, 1, 6, 3, 2, 4, 6, 2, 3});

    ASSERT_EQ(v(EndIndex()), v(v.size() - 1));

    for (size_t k = 0; k < v.size(); k++)
    {
        const size_t end_idx = v.size() - 1 - k;
        ASSERT_EQ(v(EndIndex(-static_cast<int>(k))), v(end_idx));
    }
}

TEST_F(VectorIndexingTest, VectorBeginEnd)
{
    const Vectord v0 = {1.2, 4.3, 1.5, 5.2, 3.6, 3.4, 6.7};
    const size_t vector_size = v0.size();
    const double* const vector_data = v0.getDataPointer();

    for (size_t k = 0; k < vector_size; k++)
    {
        ASSERT_EQ(v0(k), vector_data[k]);
    }
    ASSERT_EQ(vector_data[0], *(v0.begin()));
    ASSERT_EQ(vector_data[vector_size - 1], *(v0.end() - 1));

    ASSERT_EQ(&vector_data[0], v0.begin());
    ASSERT_EQ(&vector_data[vector_size], v0.end());

    Vectord nc_v0 = {1.2, 4.3, 1.5, 5.2, 3.6, 3.4, 6.7};
    const size_t nc_vector_size = nc_v0.size();
    const double* const nc_vector_data = nc_v0.getDataPointer();

    for (size_t k = 0; k < nc_vector_size; k++)
    {
        ASSERT_EQ(nc_v0(k), nc_vector_data[k]);
    }
    ASSERT_EQ(nc_vector_data[0], *(nc_v0.begin()));
    ASSERT_EQ(nc_vector_data[nc_vector_size - 1], *(nc_v0.end() - 1));

    ASSERT_EQ(&nc_vector_data[0], nc_v0.begin());
    ASSERT_EQ(&nc_vector_data[nc_vector_size], nc_v0.end());
}

TEST_F(VectorIndexingTest, VectorBeginEndIterate)
{
    const Vectord v0 = {1.2, 4.3, 1.5, 5.2, 3.6, 3.4, 6.7};
    const double* const vector_data = v0.getDataPointer();

    size_t idx = 0;
    for (double d : v0)
    {
        ASSERT_EQ(d, vector_data[idx]);
        idx++;
    }
    ASSERT_EQ(idx, v0.size());
    ASSERT_EQ(v0(v0.size() - 1), v0(idx - 1));
}

}  // namespace
}  // namespace arl
