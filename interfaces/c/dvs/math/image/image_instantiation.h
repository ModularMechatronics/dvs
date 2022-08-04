#ifndef DVS_MATH_IMAGE_IMAGE_INSTANTIATION_H_
#define DVS_MATH_IMAGE_IMAGE_INSTANTIATION_H_

#include "dvs/math/image/image_types.h"
#include "dvs/math/utils.h"

ImageC1 createImageC1(const size_t num_rows, const size_t num_cols, const DataType data_type)
{
    ImageC1 img;
    img.data_type = data_type;
    img.num_bytes_per_element = dataTypeToNumBytes(data_type);
    img.num_rows = num_rows;
    img.num_cols = num_cols;
    img.data = (uint8_t*)malloc(num_rows * num_cols * img.num_bytes_per_element);

    return img;
}

#define freeImage(img) free(img.data)

ImageC1F createImageC1F(const size_t num_rows, const size_t num_cols)
{
    ImageC1 img = createImageC1(num_rows, num_cols, DT_FLOAT);
    ImageC1F ret_img = *(ImageC1F*)(&img);
    return ret_img;
}

ImageC1D createImageC1D(const size_t num_rows, const size_t num_cols)
{
    ImageC1 img = createImageC1(num_rows, num_cols, DT_DOUBLE);
    ImageC1D ret_img = *(ImageC1D*)(&img);
    return ret_img;
}

ImageC1S8 createImageC1S8(const size_t num_rows, const size_t num_cols)
{
    ImageC1 img = createImageC1(num_rows, num_cols, DT_INT8);
    ImageC1S8 ret_img = *(ImageC1S8*)(&img);
    return ret_img;
}

ImageC1S16 createImageC1S16(const size_t num_rows, const size_t num_cols)
{
    ImageC1 img = createImageC1(num_rows, num_cols, DT_INT16);
    ImageC1S16 ret_img = *(ImageC1S16*)(&img);
    return ret_img;
}

ImageC1S32 createImageC1S32(const size_t num_rows, const size_t num_cols)
{
    ImageC1 img = createImageC1(num_rows, num_cols, DT_INT32);
    ImageC1S32 ret_img = *(ImageC1S32*)(&img);
    return ret_img;
}

ImageC1S64 createImageC1S64(const size_t num_rows, const size_t num_cols)
{
    ImageC1 img = createImageC1(num_rows, num_cols, DT_INT64);
    ImageC1S64 ret_img = *(ImageC1S64*)(&img);
    return ret_img;
}

ImageC1U8 createImageC1U8(const size_t num_rows, const size_t num_cols)
{
    ImageC1 img = createImageC1(num_rows, num_cols, DT_UINT8);
    ImageC1U8 ret_img = *(ImageC1U8*)(&img);
    return ret_img;
}

ImageC1U16 createImageC1U16(const size_t num_rows, const size_t num_cols)
{
    ImageC1 img = createImageC1(num_rows, num_cols, DT_UINT16);
    ImageC1U16 ret_img = *(ImageC1U16*)(&img);
    return ret_img;
}

ImageC1U32 createImageC1U32(const size_t num_rows, const size_t num_cols)
{
    ImageC1 img = createImageC1(num_rows, num_cols, DT_UINT32);
    ImageC1U32 ret_img = *(ImageC1U32*)(&img);
    return ret_img;
}

ImageC1U64 createImageC1U64(const size_t num_rows, const size_t num_cols)
{
    ImageC1 img = createImageC1(num_rows, num_cols, DT_UINT64);
    ImageC1U64 ret_img = *(ImageC1U64*)(&img);
    return ret_img;
}

ImageC1F toImageC1F(const ImageC1 img)
{
    ImageC1F ret_img = *(ImageC1F*)(&img);
    return ret_img;
}

ImageC1D toImageC1D(const ImageC1 img)
{
    ImageC1D ret_img = *(ImageC1D*)(&img);
    return ret_img;
}

ImageC1S8 toImageC1S8(const ImageC1 img)
{
    ImageC1S8 ret_img = *(ImageC1S8*)(&img);
    return ret_img;
}

ImageC1S16 toImageC1S16(const ImageC1 img)
{
    ImageC1S16 ret_img = *(ImageC1S16*)(&img);
    return ret_img;
}

ImageC1S32 toImageC1S32(const ImageC1 img)
{
    ImageC1S32 ret_img = *(ImageC1S32*)(&img);
    return ret_img;
}

ImageC1S64 toImageC1S64(const ImageC1 img)
{
    ImageC1S64 ret_img = *(ImageC1S64*)(&img);
    return ret_img;
}

ImageC1U8 toImageC1U8(const ImageC1 img)
{
    ImageC1U8 ret_img = *(ImageC1U8*)(&img);
    return ret_img;
}

ImageC1U16 toImageC1U16(const ImageC1 img)
{
    ImageC1U16 ret_img = *(ImageC1U16*)(&img);
    return ret_img;
}

ImageC1U32 toImageC1U32(const ImageC1 img)
{
    ImageC1U32 ret_img = *(ImageC1U32*)(&img);
    return ret_img;
}

ImageC1U64 toImageC1U64(const ImageC1 img)
{
    ImageC1U64 ret_img = *(ImageC1U64*)(&img);
    return ret_img;
}

ImageC3 createImageC3(const size_t num_rows, const size_t num_cols, const DataType data_type)
{
    ImageC3 img;
    img.data_type = data_type;
    img.num_bytes_per_element = dataTypeToNumBytes(data_type);
    img.num_rows = num_rows;
    img.num_cols = num_cols;
    img.data = (uint8_t*)malloc(3 * num_rows * num_cols * img.num_bytes_per_element);

    return img;
}

ImageC3F createImageC3F(const size_t num_rows, const size_t num_cols)
{
    ImageC3 img = createImageC3(num_rows, num_cols, DT_FLOAT);
    ImageC3F ret_img = *(ImageC3F*)(&img);
    return ret_img;
}

ImageC3D createImageC3D(const size_t num_rows, const size_t num_cols)
{
    ImageC3 img = createImageC3(num_rows, num_cols, DT_DOUBLE);
    ImageC3D ret_img = *(ImageC3D*)(&img);
    return ret_img;
}

ImageC3S8 createImageC3S8(const size_t num_rows, const size_t num_cols)
{
    ImageC3 img = createImageC3(num_rows, num_cols, DT_INT8);
    ImageC3S8 ret_img = *(ImageC3S8*)(&img);
    return ret_img;
}

ImageC3S16 createImageC3S16(const size_t num_rows, const size_t num_cols)
{
    ImageC3 img = createImageC3(num_rows, num_cols, DT_INT16);
    ImageC3S16 ret_img = *(ImageC3S16*)(&img);
    return ret_img;
}

ImageC3S32 createImageC3S32(const size_t num_rows, const size_t num_cols)
{
    ImageC3 img = createImageC3(num_rows, num_cols, DT_INT32);
    ImageC3S32 ret_img = *(ImageC3S32*)(&img);
    return ret_img;
}

ImageC3S64 createImageC3S64(const size_t num_rows, const size_t num_cols)
{
    ImageC3 img = createImageC3(num_rows, num_cols, DT_INT64);
    ImageC3S64 ret_img = *(ImageC3S64*)(&img);
    return ret_img;
}

ImageC3U8 createImageC3U8(const size_t num_rows, const size_t num_cols)
{
    ImageC3 img = createImageC3(num_rows, num_cols, DT_UINT8);
    ImageC3U8 ret_img = *(ImageC3U8*)(&img);
    return ret_img;
}

ImageC3U16 createImageC3U16(const size_t num_rows, const size_t num_cols)
{
    ImageC3 img = createImageC3(num_rows, num_cols, DT_UINT16);
    ImageC3U16 ret_img = *(ImageC3U16*)(&img);
    return ret_img;
}

ImageC3U32 createImageC3U32(const size_t num_rows, const size_t num_cols)
{
    ImageC3 img = createImageC3(num_rows, num_cols, DT_UINT32);
    ImageC3U32 ret_img = *(ImageC3U32*)(&img);
    return ret_img;
}

ImageC3U64 createImageC3U64(const size_t num_rows, const size_t num_cols)
{
    ImageC3 img = createImageC3(num_rows, num_cols, DT_UINT64);
    ImageC3U64 ret_img = *(ImageC3U64*)(&img);
    return ret_img;
}

ImageC3F toImageC3F(const ImageC3 img)
{
    ImageC3F ret_img = *(ImageC3F*)(&img);
    return ret_img;
}

ImageC3D toImageC3D(const ImageC3 img)
{
    ImageC3D ret_img = *(ImageC3D*)(&img);
    return ret_img;
}

ImageC3S8 toImageC3S8(const ImageC3 img)
{
    ImageC3S8 ret_img = *(ImageC3S8*)(&img);
    return ret_img;
}

ImageC3S16 toImageC3S16(const ImageC3 img)
{
    ImageC3S16 ret_img = *(ImageC3S16*)(&img);
    return ret_img;
}

ImageC3S32 toImageC3S32(const ImageC3 img)
{
    ImageC3S32 ret_img = *(ImageC3S32*)(&img);
    return ret_img;
}

ImageC3S64 toImageC3S64(const ImageC3 img)
{
    ImageC3S64 ret_img = *(ImageC3S64*)(&img);
    return ret_img;
}

ImageC3U8 toImageC3U8(const ImageC3 img)
{
    ImageC3U8 ret_img = *(ImageC3U8*)(&img);
    return ret_img;
}

ImageC3U16 toImageC3U16(const ImageC3 img)
{
    ImageC3U16 ret_img = *(ImageC3U16*)(&img);
    return ret_img;
}

ImageC3U32 toImageC3U32(const ImageC3 img)
{
    ImageC3U32 ret_img = *(ImageC3U32*)(&img);
    return ret_img;
}

ImageC3U64 toImageC3U64(const ImageC3 img)
{
    ImageC3U64 ret_img = *(ImageC3U64*)(&img);
    return ret_img;
}


#endif // DVS_MATH_IMAGE_IMAGE_INSTANTIATION_H_
