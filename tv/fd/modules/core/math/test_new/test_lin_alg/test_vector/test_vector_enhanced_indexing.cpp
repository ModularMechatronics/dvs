
class NewTest1 : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(NewTest1, VectorIndexingTest0)
{
    const Vectord v0 = {1, 6, -1, 7, 4, 13, 9, 2, 0};

    const Vectord v1 = v0({2, 6});
    Vectord v2 = v0({2, 6});

    ASSERT_EQ(v1.size(), 5);
    ASSERT_EQ(v2.size(), 5);
    for (size_t k = 0; k < 5; k++)
    {
        ASSERT_EQ(v0(k + 2), v1(k));
        ASSERT_EQ(v0(k + 2), v2(k));
    }
    v2 = v0({1, 6});

    ASSERT_EQ(v2.size(), 6);
    for (size_t k = 0; k < 6; k++)
    {
        ASSERT_EQ(v0(k + 1), v2(k));
    }
}

TEST_F(NewTest1, VectorIndexingTest1)
{
    Vectord v0 = {1, 6, -1, 7, 4, 13, 9, 2, 0};

    const Vectord v1 = v0({2, 6});
    Vectord v2 = v0({2, 6});

    ASSERT_EQ(v1.size(), 5);
    ASSERT_EQ(v2.size(), 5);

    for (size_t k = 0; k < 5; k++)
    {
        ASSERT_EQ(v0(k + 2), v1(k));
        ASSERT_EQ(v0(k + 2), v2(k));
    }
    v2 = v0({1, 6});

    ASSERT_EQ(v2.size(), 6);
    for (size_t k = 0; k < 6; k++)
    {
        ASSERT_EQ(v0(k + 1), v2(k));
    }
}

TEST_F(NewTest1, VectorIndexingTest2)
{
    Vectord v0 = {1, 6, -1, 7, 4, 13, 9, 2, 0};
    Vectord v1 = {12, 16, 12, 17, 14, 12};

    v1({1, 4}) = v0({2, 5});
    for (size_t idx : Vector<size_t>({1, 2, 3, 4}))
    {
        ASSERT_EQ(v0(idx), v1(idx + 1));
    }

    ASSERT_EQ(v0.size(), 9);
    ASSERT_EQ(v1.size(), 6);
}

TEST_F(NewTest1, VectorIndexingTest3)
{
    const Vectord v0 = {1, 6, -1, 7, 4, 13, 9, 2, 0};
    Vectord v1 = {12, 16, 12, 17, 14, 12};

    v1({1, 4}) = v0({2, 5});
    for (size_t idx : Vector<size_t>({1, 2, 3, 4}))
    {
        ASSERT_EQ(v0(idx), v1(idx + 1));
    }

    ASSERT_EQ(v0.size(), 9);
    ASSERT_EQ(v1.size(), 6);
}

TEST_F(NewTest1, VectorIndexingTest4)
{
    const Vectord v0 = {1, 6, 1, 7, 4, 13, 9, 2, 0, 6, 2, 6, 3, 1};
    Vectord v1 = {12, 16, 12, 17, 14, 12, 15, 13, 10, 12, 16};
    const Vectord v1_copy = v1;

    const Vector<size_t> indices0 = {2, 5, 2, 1, 7};

    v1({1, 5}) = v0(indices0);
    const Vectord v2 = v0(indices0);
    Vectord v3 = v0(indices0);

    for (size_t k = 0; k < indices0.size(); k++)
    {
        ASSERT_EQ(v2(k), v0(indices0(k)));
        ASSERT_EQ(v3(k), v0(indices0(k)));
    }

    for (size_t k = 0; k < v1.size(); k++)
    {
        if ((k >= 1) && (k <= 5))
        {
            ASSERT_EQ(v1(k), v0(indices0(k - 1)));
        }
        else
        {
            ASSERT_EQ(v1(k), v1_copy(k));
        }
    }

    ASSERT_EQ(v0.size(), 14);
    ASSERT_EQ(v1.size(), 11);
    ASSERT_EQ(v2.size(), indices.size());
    ASSERT_EQ(v3.size(), indices.size());
}

TEST_F(NewTest1, VectorIndexingTest4)
{
    const Vectord v0 = {1, 6, 1, 7, 4, 13, 9, 2, 0, 6, 2, 6, 3, 1};
    Vectord v1 = {12, 16, 12, 17, 14, 12, 15, 13, 10, 12, 16};
    const Vectord v1_copy = v1;

    const Vector<size_t> indices0 = {2, 5, 2, 1, 7};
    const Vector<size_t> indices1 = {3, 1, 5, 2, 4};

    v1(indices1) = v0(indices0);

    for (size_t k = 0; k < v1.size(); k++)
    {
        if ((k >= 1) && (k <= 5))
        {
            ASSERT_EQ(v1(indices1(k)), v0(indices0(k - 1)));
        }
        else
        {
            ASSERT_EQ(v1(k), v1_copy(k));
        }
    }

    ASSERT_EQ(v0.size(), 14);
    ASSERT_EQ(v1.size(), 11);
}

TEST_F(NewTest1, VectorIndexingTest4)
{
    const Vectord v0 = {1, 6, 1, 7, 4, 13, 9, 2, 0, 6, 2, 6, 3, 1};
    Vectord v1 = {12, 16, 12, 17, 14, 12, 15, 13, 10, 12, 16};
    const Vectord v1_copy = v1;

    const Vector<size_t> indices0 = {2, 5, 2, 1, 7};
    const Vector<bool> indices1 = {0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0};
    const Vector<size_t> indices2 = {1, 3, 4, 7, 9};

    v1(indices1) = v0(indices0);

    for (size_t k = 0; k < v1.size(); k++)
    {
        if (indices1(k))
        {
            ASSERT_EQ(v1(indices2(k)), v0(indices0(k - 1)));
        }
        else
        {
            ASSERT_EQ(v1(k), v1_copy(k));
        }
    }

    ASSERT_EQ(v0.size(), 14);
    ASSERT_EQ(v1.size(), 11);
}

TEST_F(NewTest1, VectorIndexingTest4)
{
    const Vectord v0 = {1, 6, 1, 7, 4, 13, 9, 2, 0, 6, 2, 6, 3, 1};
    Vectord v1 = {12, 16, 12, 17, 14, 12, 15, 13, 10, 12, 16};
    const Vectord v1_copy = v1;

    const Vector<bool> indices0 = {0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0};
    const Vector<size_t> indices1 = {2, 5, 2, 1, 7};
    const Vector<size_t> indices2 = {2, 5, 6, 8, 11};

    v1(indices1) = v0(indices0);

    size_t idx = 0;

    for (size_t k = 0; k < v1.size(); k++)
    {
        if (indices1.contains(k))
        {
            ASSERT_EQ(v1(indices1(idx)), v0(indices2(idx)));
            idx++;
        }
        else
        {
            ASSERT_EQ(v1(k), v1_copy(k));
        }
    }

    ASSERT_EQ(v0.size(), 14);
    ASSERT_EQ(v1.size(), 11);
}
