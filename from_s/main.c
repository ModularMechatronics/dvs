// #include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>
#include <assert.h>


typedef enum
{
    UNKNOWN = 0,
    COLOR = 1,
    ALPHA = 2,
    LINE_WIDTH = 3,
    PROPERTY_TYPE_LAST_PROP = 4
} PropertyType;

typedef struct
{
    PropertyType type;
    char data[256];
} Property;

typedef enum
{
    FLOAT,
    DOUBLE,
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    INT8,
    INT16,
    INT32,
    INT64
} DataType;

size_t dataTypeToNumBytes(const DataType dt)
{
    switch (dt)
    {
        case FLOAT:
            return sizeof(float);
            break;
        case DOUBLE:
            return sizeof(double);
            break;
        case UINT8:
            return sizeof(uint8_t);
            break;
        case UINT16:
            return sizeof(uint16_t);
            break;
        case UINT32:
            return sizeof(uint32_t);
            break;
        case UINT64:
            return sizeof(uint64_t);
            break;
        case INT8:
            return sizeof(int8_t);
            break;
        case INT16:
            return sizeof(int16_t);
            break;
        case INT32:
            return sizeof(int32_t);
            break;
        case INT64:
            return sizeof(int64_t);
            break;
        default:
            exit(-1); // Invalid data type
            break;
    }
}

typedef struct
{
    DataType data_type;
    size_t num_rows;
    size_t num_cols;
    uint8_t* data;

} Matrix;

typedef struct
{
    DataType data_type;
    size_t num_elements;
    uint8_t *data;

} Vector;

Matrix createMatrix(const size_t num_rows, const size_t num_cols, const DataType data_type)
{
    Matrix mat;
    mat.data_type = data_type;
    mat.num_rows = num_rows;
    mat.num_cols = num_cols;

    mat.data = (uint8_t*)malloc(num_rows * num_cols * dataTypeToNumBytes(data_type));

    return mat;
}

#define createFloatMatrix(num_rows, num_cols) createMatrix(num_rows, num_cols, FLOAT)

Vector createVector(const size_t num_elements, const DataType data_type)
{
    Vector vec;
    vec.data_type = data_type;
    vec.num_elements = num_elements;

    vec.data = (uint8_t*)malloc(num_elements * dataTypeToNumBytes(data_type));

    return vec;
}

void setFloatMatElementAt(Matrix m, const size_t row, const size_t col, const float val)
{
    assert(m.data_type == FLOAT && "Invalid data type for matrix!");
    float * const data_ptr = (float* const)(m.data);
    const size_t idx = row * m.num_cols + col;
    data_ptr[idx] = val;
}

float getFloatMatElementAt(const Matrix m, const size_t row, const size_t col)
{
    assert(m.data_type == FLOAT && "Invalid data type for matrix!");
    const float * const data_ptr = (const float* const)(m.data);
    const size_t idx = row * m.num_cols + col;
    return data_ptr[idx];
}

void setFloatVecElementAt(Vector v, const size_t idx, const float val)
{
    assert(v.data_type == FLOAT && "Invalid data type for vector!");
    float * const data_ptr = (float* const)(v.data);
    data_ptr[idx] = val;
}

float getFloatVecElementAt(const Vector v, const size_t idx)
{
    assert(v.data_type == FLOAT && "Invalid data type for vector!");
    const float * const data_ptr = (const float* const)(v.data);
    return data_ptr[idx];
}

#define freeVector(vec) {  \
    free(vec.data);        \
    vec.num_elements = 0;  \
}

#define freeMatrix(mat) {  \
    free(mat.data);        \
    mat.num_rows = 0;      \
    mat.num_cols = 0;      \
}


Property getLastProperty(void)
{
    Property last_prop;
    last_prop.type = PROPERTY_TYPE_LAST_PROP;

    return last_prop;
}

void plotInternal(const Vector x, const Vector y, const Property first_prop, ...)
{
    printf("x: ");
    for(size_t k = 0; k < x.num_elements; k++)
    {
        printf("%f, ", getFloatVecElementAt(x, k));
    }
    printf("\n");

    printf("x: ");
    for(size_t k = 0; k < y.num_elements; k++)
    {
        printf("%f, ", getFloatVecElementAt(y, k));
    }
    printf("\n");
    
    va_list args;
    va_start(args, first_prop);

    Property prp = first_prop;
    while(prp.type != PROPERTY_TYPE_LAST_PROP)
    {
        printf("Property: %s\n", prp.data);
        prp = va_arg(args, Property);
    }
}

void surfInternal(const Matrix x, const Matrix y, const Matrix z, const Property first_prop, ...)
{
    va_list args;
    va_start(args, first_prop);

    Property prp = first_prop;
    while(prp.type != PROPERTY_TYPE_LAST_PROP)
    {
        printf("Property: %s\n", prp.data);
        prp = va_arg(args, Property);
    }
}

// Use as plot(const Vector x, const Vector y, Property p0, Property p1...)
#define plot(x, y, ...) plotInternal(x, y, __VA_ARGS__, getLastProperty())

// Use as surf(const Matrix x, const Matrix y, const Matrix z, Property p0, Property p1...)
#define surf(x, y, z, ...) surfInternal(x, y, z, __VA_ARGS__, getLastProperty())

Property Color(const uint8_t red, const uint8_t green, const uint8_t blue)
{
    Property clr;
    clr.type = COLOR;
    strcpy(clr.data, "Color");

    return clr;
}


Property RED(void)
{
    return Color(255, 0, 0);
}

Property Alpha(const float alpha_val)
{
    Property alpha;
    alpha.type = ALPHA;
    strcpy(alpha.data, "Alpha");

    return alpha;
}

int main()
{
    Property prop2;

    strcpy(prop2.data, "Prop2");

    prop2.type = LINE_WIDTH;

    Matrix xm = createMatrix(11, 12, FLOAT);
    Matrix ym = createMatrix(11, 12, FLOAT);
    Matrix zm = createMatrix(11, 12, FLOAT);

    Vector x = createVector(7, FLOAT);
    Vector y = createVector(7, FLOAT);

    for(size_t k = 0; k < y.num_elements; k++)
    {
        setFloatVecElementAt(x, k, k * 3.0f + 1.0f);
        setFloatVecElementAt(y, k, k * 2.0f);
    }

    plot(x, y, Color(0, 2, 44), Alpha(0.1f), prop2);
    surf(xm, ym, zm, Color(0, 2, 44), Alpha(0.1f), prop2);

    freeVector(x);
    freeVector(y);

    printf("xm: [%d, %d]\n", xm.num_rows, xm.num_cols);
    printf("xm: [%d, %d]\n", ym.num_rows, ym.num_cols);
    printf("xm: [%d, %d]\n", zm.num_rows, zm.num_cols);

    freeMatrix(xm);
    freeMatrix(ym);
    freeMatrix(zm);
    
    printf("xm: [%d, %d]\n", xm.num_rows, xm.num_cols);
    printf("xm: [%d, %d]\n", ym.num_rows, ym.num_cols);
    printf("xm: [%d, %d]\n", zm.num_rows, zm.num_cols);

    return 0;
}
