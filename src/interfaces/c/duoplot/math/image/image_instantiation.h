#ifndef DUOPLOT_MATH_IMAGE_IMAGE_INSTANTIATION_H_
#define DUOPLOT_MATH_IMAGE_IMAGE_INSTANTIATION_H_

#include "duoplot/math/image/image_types.h"
#include "duoplot/math/utils.h"
#include "duoplot/pp.h"

DUOPLOT_WEAK duoplot_ImageGray duoplot_createImageGray(const size_t num_rows,
                                                       const size_t num_cols,
                                                       const duoplot_internal_DataType data_type)
{
    duoplot_ImageGray img;
    img.data_type = data_type;
    img.num_bytes_per_element = duoplot_internal_dataTypeToNumBytes(data_type);
    img.num_rows = num_rows;
    img.num_cols = num_cols;
    img.data = (uint8_t*)malloc(num_rows * num_cols * img.num_bytes_per_element);

    return img;
}

#define duoplot_freeImage(img) free(img.data)

DUOPLOT_WEAK duoplot_ImageGrayF duoplot_createImageGrayF(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageGray img = duoplot_createImageGray(num_rows, num_cols, DUOPLOT_INTERNAL_DT_FLOAT);
    duoplot_ImageGrayF ret_img = *(duoplot_ImageGrayF*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayD duoplot_createImageGrayD(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageGray img = duoplot_createImageGray(num_rows, num_cols, DUOPLOT_INTERNAL_DT_DOUBLE);
    duoplot_ImageGrayD ret_img = *(duoplot_ImageGrayD*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayS8 duoplot_createImageGrayS8(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageGray img = duoplot_createImageGray(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT8);
    duoplot_ImageGrayS8 ret_img = *(duoplot_ImageGrayS8*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayS16 duoplot_createImageGrayS16(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageGray img = duoplot_createImageGray(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT16);
    duoplot_ImageGrayS16 ret_img = *(duoplot_ImageGrayS16*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayS32 duoplot_createImageGrayS32(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageGray img = duoplot_createImageGray(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT32);
    duoplot_ImageGrayS32 ret_img = *(duoplot_ImageGrayS32*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayS64 duoplot_createImageGrayS64(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageGray img = duoplot_createImageGray(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT64);
    duoplot_ImageGrayS64 ret_img = *(duoplot_ImageGrayS64*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayU8 duoplot_createImageGrayU8(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageGray img = duoplot_createImageGray(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT8);
    duoplot_ImageGrayU8 ret_img = *(duoplot_ImageGrayU8*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayU16 duoplot_createImageGrayU16(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageGray img = duoplot_createImageGray(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT16);
    duoplot_ImageGrayU16 ret_img = *(duoplot_ImageGrayU16*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayU32 duoplot_createImageGrayU32(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageGray img = duoplot_createImageGray(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT32);
    duoplot_ImageGrayU32 ret_img = *(duoplot_ImageGrayU32*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayU64 duoplot_createImageGrayU64(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageGray img = duoplot_createImageGray(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT64);
    duoplot_ImageGrayU64 ret_img = *(duoplot_ImageGrayU64*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayF duoplot_toImageGrayF(const duoplot_ImageGray img)
{
    duoplot_ImageGrayF ret_img = *(duoplot_ImageGrayF*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayD duoplot_toImageGrayD(const duoplot_ImageGray img)
{
    duoplot_ImageGrayD ret_img = *(duoplot_ImageGrayD*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayS8 duoplot_toImageGrayS8(const duoplot_ImageGray img)
{
    duoplot_ImageGrayS8 ret_img = *(duoplot_ImageGrayS8*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayS16 duoplot_toImageGrayS16(const duoplot_ImageGray img)
{
    duoplot_ImageGrayS16 ret_img = *(duoplot_ImageGrayS16*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayS32 duoplot_toImageGrayS32(const duoplot_ImageGray img)
{
    duoplot_ImageGrayS32 ret_img = *(duoplot_ImageGrayS32*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayS64 duoplot_toImageGrayS64(const duoplot_ImageGray img)
{
    duoplot_ImageGrayS64 ret_img = *(duoplot_ImageGrayS64*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayU8 duoplot_toImageGrayU8(const duoplot_ImageGray img)
{
    duoplot_ImageGrayU8 ret_img = *(duoplot_ImageGrayU8*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayU16 duoplot_toImageGrayU16(const duoplot_ImageGray img)
{
    duoplot_ImageGrayU16 ret_img = *(duoplot_ImageGrayU16*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayU32 duoplot_toImageGrayU32(const duoplot_ImageGray img)
{
    duoplot_ImageGrayU32 ret_img = *(duoplot_ImageGrayU32*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageGrayU64 duoplot_toImageGrayU64(const duoplot_ImageGray img)
{
    duoplot_ImageGrayU64 ret_img = *(duoplot_ImageGrayU64*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGB duoplot_createImageRGB(const size_t num_rows,
                                                     const size_t num_cols,
                                                     const duoplot_internal_DataType data_type)
{
    duoplot_ImageRGB img;
    img.data_type = data_type;
    img.num_bytes_per_element = duoplot_internal_dataTypeToNumBytes(data_type);
    img.num_rows = num_rows;
    img.num_cols = num_cols;
    img.data = (uint8_t*)malloc(3 * num_rows * num_cols * img.num_bytes_per_element);

    return img;
}

DUOPLOT_WEAK duoplot_ImageRGBF duoplot_createImageRGBF(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageRGB img = duoplot_createImageRGB(num_rows, num_cols, DUOPLOT_INTERNAL_DT_FLOAT);
    duoplot_ImageRGBF ret_img = *(duoplot_ImageRGBF*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBD duoplot_createImageRGBD(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageRGB img = duoplot_createImageRGB(num_rows, num_cols, DUOPLOT_INTERNAL_DT_DOUBLE);
    duoplot_ImageRGBD ret_img = *(duoplot_ImageRGBD*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBS8 duoplot_createImageRGBS8(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageRGB img = duoplot_createImageRGB(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT8);
    duoplot_ImageRGBS8 ret_img = *(duoplot_ImageRGBS8*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBS16 duoplot_createImageRGBS16(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageRGB img = duoplot_createImageRGB(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT16);
    duoplot_ImageRGBS16 ret_img = *(duoplot_ImageRGBS16*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBS32 duoplot_createImageRGBS32(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageRGB img = duoplot_createImageRGB(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT32);
    duoplot_ImageRGBS32 ret_img = *(duoplot_ImageRGBS32*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBS64 duoplot_createImageRGBS64(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageRGB img = duoplot_createImageRGB(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT64);
    duoplot_ImageRGBS64 ret_img = *(duoplot_ImageRGBS64*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBU8 duoplot_createImageRGBU8(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageRGB img = duoplot_createImageRGB(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT8);
    duoplot_ImageRGBU8 ret_img = *(duoplot_ImageRGBU8*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBU16 duoplot_createImageRGBU16(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageRGB img = duoplot_createImageRGB(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT16);
    duoplot_ImageRGBU16 ret_img = *(duoplot_ImageRGBU16*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBU32 duoplot_createImageRGBU32(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageRGB img = duoplot_createImageRGB(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT32);
    duoplot_ImageRGBU32 ret_img = *(duoplot_ImageRGBU32*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBU64 duoplot_createImageRGBU64(const size_t num_rows, const size_t num_cols)
{
    duoplot_ImageRGB img = duoplot_createImageRGB(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT64);
    duoplot_ImageRGBU64 ret_img = *(duoplot_ImageRGBU64*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBF duoplot_toImageRGBF(const duoplot_ImageRGB img)
{
    duoplot_ImageRGBF ret_img = *(duoplot_ImageRGBF*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBD duoplot_toImageRGBD(const duoplot_ImageRGB img)
{
    duoplot_ImageRGBD ret_img = *(duoplot_ImageRGBD*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBS8 duoplot_toImageRGBS8(const duoplot_ImageRGB img)
{
    duoplot_ImageRGBS8 ret_img = *(duoplot_ImageRGBS8*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBS16 duoplot_toImageRGBS16(const duoplot_ImageRGB img)
{
    duoplot_ImageRGBS16 ret_img = *(duoplot_ImageRGBS16*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBS32 duoplot_toImageRGBS32(const duoplot_ImageRGB img)
{
    duoplot_ImageRGBS32 ret_img = *(duoplot_ImageRGBS32*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBS64 duoplot_toImageRGBS64(const duoplot_ImageRGB img)
{
    duoplot_ImageRGBS64 ret_img = *(duoplot_ImageRGBS64*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBU8 duoplot_toImageRGBU8(const duoplot_ImageRGB img)
{
    duoplot_ImageRGBU8 ret_img = *(duoplot_ImageRGBU8*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBU16 duoplot_toImageRGBU16(const duoplot_ImageRGB img)
{
    duoplot_ImageRGBU16 ret_img = *(duoplot_ImageRGBU16*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBU32 duoplot_toImageRGBU32(const duoplot_ImageRGB img)
{
    duoplot_ImageRGBU32 ret_img = *(duoplot_ImageRGBU32*)(&img);
    return ret_img;
}

DUOPLOT_WEAK duoplot_ImageRGBU64 duoplot_toImageRGBU64(const duoplot_ImageRGB img)
{
    duoplot_ImageRGBU64 ret_img = *(duoplot_ImageRGBU64*)(&img);
    return ret_img;
}

#endif  // DUOPLOT_MATH_IMAGE_IMAGE_INSTANTIATION_H_
