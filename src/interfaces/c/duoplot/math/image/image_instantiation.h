#ifndef DUOPLOT_MATH_IMAGE_IMAGE_INSTANTIATION_H_
#define DUOPLOT_MATH_IMAGE_IMAGE_INSTANTIATION_H_

#include "duoplot/math/image/image_types.h"
#include "duoplot/math/utils.h"
#include "duoplot/pp.h"

DUOPLOT_WEAK duoplot_ImageC1 duoplot_createImageC1(const size_t num_rows,
                                                   const size_t num_cols,
                                                   const DataType data_type)
{
    duoplot_ImageC1 img;
    img.data_type = data_type;
    img.num_bytes_per_element = duoplot_internal_dataTypeToNumBytes(data_type);
    img.num_rows = num_rows;
    img.num_cols = num_cols;
    img.data = (uint8_t*)malloc(num_rows * num_cols * img.num_bytes_per_element);

    return img;
}

#define duoplot_freeImage(img) free(img.data)

DUOPLOT_WEAK duoplot_ImageC1F duoplot_createImageC1F(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC1 img = duoplot_createImageC1(num_rows, num_cols, DUOPLOT_INTERNAL_DT_FLOAT);
    duoplot_ImageC1F ret_img = *(duoplot_ImageC1F*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1D duoplot_createImageC1D(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC1 img = duoplot_createImageC1(num_rows, num_cols, DUOPLOT_INTERNAL_DT_DOUBLE);
    duoplot_ImageC1D ret_img = *(duoplot_ImageC1D*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1S8 duoplot_createImageC1S8(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC1 img = duoplot_createImageC1(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT8);
    duoplot_ImageC1S8 ret_img = *(duoplot_ImageC1S8*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1S16 duoplot_createImageC1S16(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC1 img = duoplot_createImageC1(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT16);
    duoplot_ImageC1S16 ret_img = *(duoplot_ImageC1S16*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1S32 duoplot_createImageC1S32(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC1 img = duoplot_createImageC1(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT32);
    duoplot_ImageC1S32 ret_img = *(duoplot_ImageC1S32*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1S64 duoplot_createImageC1S64(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC1 img = duoplot_createImageC1(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT64);
    duoplot_ImageC1S64 ret_img = *(duoplot_ImageC1S64*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1U8 duoplot_createImageC1U8(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC1 img = duoplot_createImageC1(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT8);
    duoplot_ImageC1U8 ret_img = *(duoplot_ImageC1U8*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1U16 duoplot_createImageC1U16(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC1 img = duoplot_createImageC1(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT16);
    duoplot_ImageC1U16 ret_img = *(duoplot_ImageC1U16*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1U32 duoplot_createImageC1U32(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC1 img = duoplot_createImageC1(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT32);
    duoplot_ImageC1U32 ret_img = *(duoplot_ImageC1U32*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1U64 duoplot_createImageC1U64(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC1 img = duoplot_createImageC1(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT64);
    duoplot_ImageC1U64 ret_img = *(duoplot_ImageC1U64*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1F duoplot_toImageC1F(const duoplot_ImageC1 img)
{
    duoplot_ImageC1F ret_img = *(duoplot_ImageC1F*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1D duoplot_toImageC1D(const duoplot_ImageC1 img)
{
    duoplot_ImageC1D ret_img = *(duoplot_ImageC1D*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1S8 duoplot_toImageC1S8(const duoplot_ImageC1 img)
{
    duoplot_ImageC1S8 ret_img = *(duoplot_ImageC1S8*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1S16 duoplot_toImageC1S16(const duoplot_ImageC1 img)
{
    duoplot_ImageC1S16 ret_img = *(duoplot_ImageC1S16*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1S32 duoplot_toImageC1S32(const duoplot_ImageC1 img)
{
    duoplot_ImageC1S32 ret_img = *(duoplot_ImageC1S32*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1S64 duoplot_toImageC1S64(const duoplot_ImageC1 img)
{
    duoplot_ImageC1S64 ret_img = *(duoplot_ImageC1S64*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1U8 duoplot_toImageC1U8(const duoplot_ImageC1 img)
{
    duoplot_ImageC1U8 ret_img = *(duoplot_ImageC1U8*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1U16 duoplot_toImageC1U16(const duoplot_ImageC1 img)
{
    duoplot_ImageC1U16 ret_img = *(duoplot_ImageC1U16*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1U32 duoplot_toImageC1U32(const duoplot_ImageC1 img)
{
    duoplot_ImageC1U32 ret_img = *(duoplot_ImageC1U32*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC1U64 duoplot_toImageC1U64(const duoplot_ImageC1 img)
{
    duoplot_ImageC1U64 ret_img = *(duoplot_ImageC1U64*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3 duoplot_createImageC3(const size_t num_rows,
                                                   const size_t num_cols,
                                                   const DataType data_type)
{
    duoplot_ImageC3 img;
    img.data_type = data_type;
    img.num_bytes_per_element = duoplot_internal_dataTypeToNumBytes(data_type);
    img.num_rows = num_rows;
    img.num_cols = num_cols;
    img.data = (uint8_t*)malloc(3 * num_rows * num_cols * img.num_bytes_per_element);

    return img;
}

DUOPLOT_WEAK duoplot_ImageC3F duoplot_createImageC3F(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC3 img = duoplot_createImageC3(num_rows, num_cols, DUOPLOT_INTERNAL_DT_FLOAT);
    duoplot_ImageC3F ret_img = *(duoplot_ImageC3F*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3D duoplot_createImageC3D(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC3 img = duoplot_createImageC3(num_rows, num_cols, DUOPLOT_INTERNAL_DT_DOUBLE);
    duoplot_ImageC3D ret_img = *(duoplot_ImageC3D*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3S8 duoplot_createImageC3S8(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC3 img = duoplot_createImageC3(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT8);
    duoplot_ImageC3S8 ret_img = *(duoplot_ImageC3S8*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3S16 duoplot_createImageC3S16(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC3 img = duoplot_createImageC3(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT16);
    duoplot_ImageC3S16 ret_img = *(duoplot_ImageC3S16*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3S32 duoplot_createImageC3S32(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC3 img = duoplot_createImageC3(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT32);
    duoplot_ImageC3S32 ret_img = *(duoplot_ImageC3S32*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3S64 duoplot_createImageC3S64(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC3 img = duoplot_createImageC3(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT64);
    duoplot_ImageC3S64 ret_img = *(duoplot_ImageC3S64*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3U8 duoplot_createImageC3U8(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC3 img = duoplot_createImageC3(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT8);
    duoplot_ImageC3U8 ret_img = *(duoplot_ImageC3U8*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3U16 duoplot_createImageC3U16(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC3 img = duoplot_createImageC3(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT16);
    duoplot_ImageC3U16 ret_img = *(duoplot_ImageC3U16*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3U32 duoplot_createImageC3U32(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC3 img = duoplot_createImageC3(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT32);
    duoplot_ImageC3U32 ret_img = *(duoplot_ImageC3U32*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3U64 duoplot_createImageC3U64(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageC3 img = duoplot_createImageC3(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT64);
    duoplot_ImageC3U64 ret_img = *(duoplot_ImageC3U64*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3F duoplot_toImageC3F(const duoplot_ImageC3 img)
{
    duoplot_ImageC3F ret_img = *(duoplot_ImageC3F*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3D duoplot_toImageC3D(const duoplot_ImageC3 img)
{
    duoplot_ImageC3D ret_img = *(duoplot_ImageC3D*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3S8 duoplot_toImageC3S8(const duoplot_ImageC3 img)
{
    duoplot_ImageC3S8 ret_img = *(duoplot_ImageC3S8*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3S16 duoplot_toImageC3S16(const duoplot_ImageC3 img)
{
    duoplot_ImageC3S16 ret_img = *(duoplot_ImageC3S16*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3S32 duoplot_toImageC3S32(const duoplot_ImageC3 img)
{
    duoplot_ImageC3S32 ret_img = *(duoplot_ImageC3S32*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3S64 duoplot_toImageC3S64(const duoplot_ImageC3 img)
{
    duoplot_ImageC3S64 ret_img = *(duoplot_ImageC3S64*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3U8 duoplot_toImageC3U8(const duoplot_ImageC3 img)
{
    duoplot_ImageC3U8 ret_img = *(duoplot_ImageC3U8*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3U16 duoplot_toImageC3U16(const duoplot_ImageC3 img)
{
    duoplot_ImageC3U16 ret_img = *(duoplot_ImageC3U16*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3U32 duoplot_toImageC3U32(const duoplot_ImageC3 img)
{
    duoplot_ImageC3U32 ret_img = *(duoplot_ImageC3U32*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageC3U64 duoplot_toImageC3U64(const duoplot_ImageC3 img)
{
    duoplot_ImageC3U64 ret_img = *(duoplot_ImageC3U64*)(&img);
    return ret_img;
}

#endif  // DUOPLOT_MATH_IMAGE_IMAGE_INSTANTIATION_H_
