#ifndef DVS_ENUMS_H_
#define DVS_ENUMS_H_

#include <stdint.h>

namespace dvs
{
enum class Attribute : uint16_t
{
    FUNCTION,
    NUM_BUFFERS_REQUIRED,
    NUM_BYTES,
    DATA_STRUCTURE,
    BYTES_PER_ELEMENT,
    DATA_TYPE,
    NUM_ELEMENTS,
    DIMENSION_2D,
    HAS_PAYLOAD,
    AZIMUTH,
    ELEVATION,
    AXES_DIMENSIONS,
    AXIS_MIN_MAX_VEC,
    LINEWIDTH,
    POINT_SIZE,
    FACE_COLOR,
    POS2D,
    EDGE_COLOR,
    COLOR,
    FIGURE_NUM,
    ALPHA,
    LINE_STYLE,
    NAME,
    COLOR_MAP,
    UNKNOWN,
    PERSISTENT
};

enum class Function : uint8_t
{
    LINE3D,
    PLANE_XY,
    PLANE_XZ,
    PLANE_YZ,
    GRID_ON,
    GRID_OFF,
    PLOT2,
    PLOT3,
    SCATTER2,
    SCATTER3,
    LINE_BETWEEN_POINTS_3D,
    POLYGON_FROM_4_POINTS,
    FIGURE,
    CLEAR,
    HOLD_ON,
    HOLD_OFF,
    POSITION,
    SURF,
    IM_SHOW,
    AXES,
    VIEW,
    UNKNOWN,
    SOFT_CLEAR
};

enum class DataType : uint8_t
{
    UNKNOWN,
    FLOAT,
    DOUBLE,
    INT8,
    INT16,
    INT32,
    INT64,
    UINT8,
    UINT16,
    UINT32,
    UINT64
};



enum class DataStructure : uint8_t
{
    MATRIX,
    VECTOR,
    IMAGE,
    TEXT
};
}


#endif
