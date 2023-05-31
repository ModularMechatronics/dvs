
void testMisc()
{
    const size_t num_elements = 10;
    Vector v = createVector(num_elements, DT_FLOAT);
    VectorF v2 = toVectorF(v);
    VectorF v1 = createVectorF(num_elements);
    VectorF vq = vector_linspaceFromPointsAndCountF(0.1, 0.8, num_elements);

    for (size_t k = 0; k < num_elements; k++)
    {
        v2.data[k] = ((float)k) * 1.1f;
        printf("Data: %f\n", vq.data[k]);
    }

    printf("Num elements: %lu\n", vq.num_elements);

    freeVector(v);
}
