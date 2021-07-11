#ifndef DVS_ENUMS_H_
#define DVS_ENUMS_H_

#include <stdint.h>

namespace dvs
{
namespace internal
{

enum class FunctionHeaderObjectType : uint16_t
{
    FUNCTION,
    NUM_BUFFERS_REQUIRED,
    NUM_BYTES,
    DATA_STRUCTURE,
    BYTES_PER_ELEMENT,
    DATA_TYPE,
    NUM_CHANNELS,
    NUM_ELEMENTS,
    NUM_VERTICES,
    NUM_INDICES,
    DIMENSION_2D,
    HAS_PAYLOAD,
    AZIMUTH,
    ELEVATION,
    AXIS_MIN_MAX_VEC,
    POS2D,
    FIGURE_NUM,
    PARENT_NAME,
    PARENT_TYPE,
    ELEMENT_NAME,
    GUI_ELEMENT_TYPE,
    PROPERTY,
    UNKNOWN
};

enum class PropertyType : uint8_t
{
    LINE_WIDTH,
    ALPHA,
    NAME,
    LINE_STYLE,
    COLOR,
    EDGE_COLOR,
    FACE_COLOR,
    COLOR_MAP,
    POINT_SIZE,
    PERSISTENT,
    UNKNOWN
};

enum class Function : uint8_t
{
    DRAW_LINE3D,
    PLANE_XY,
    PLANE_XZ,
    PLANE_YZ,
    GRID_ON,
    GRID_OFF,
    PLOT2,
    PLOT3,
    SET_CURRENT_ELEMENT,
    CREATE_NEW_ELEMENT,
    NEW_ELEMENT,
    SCATTER2,
    SCATTER3,
    DRAW_LINE_BETWEEN_POINTS_3D,
    POLYGON_FROM_4_POINTS,
    DRAW_TRIANGLES_3D,
    DRAW_TRIANGLE_3D,
    DRAW_MESH,
    HOLD_ON,
    HOLD_OFF,
    POSITION,
    SURF,
    IM_SHOW,
    AXES_2D,
    AXES_3D,
    VIEW,
    CLEAR,
    SOFT_CLEAR,
    UNKNOWN
};

enum class DataType : uint8_t
{
    FLOAT,
    DOUBLE,
    INT8,
    INT16,
    INT32,
    INT64,
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    UNKNOWN
};

enum class DataStructure : uint8_t
{
    MATRIX,
    VECTOR,
    IMAGE,
    TEXT,
    UNKNOWN
};

enum class ColorMapType : uint8_t
{
    JET,
    RAINBOW,
    MAGMA,
    VIRIDIS,
    UNKNOWN
};

}

namespace properties
{
    constexpr internal::PropertyType PERSISTENT = internal::PropertyType::PERSISTENT;
}

enum class ElementType : uint8_t
{
    PLOT,
    IMAGE,
    WORLD_3D,
    BUTTON,
    TEXT_FIELD,
    UNKNOWN
};

enum class ElementParent : uint8_t
{
    TAB,
    WINDOW,
    UNKNOWN
};

}


#endif
