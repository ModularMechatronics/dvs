#ifndef DVS_ENUMERATIONS_H_
#define DVS_ENUMERATIONS_H_

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
    NUM_OBJECTS,
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
    LINE_STRIP,
    PERSISTENT,
    SCATTER_STYLE,
    UNKNOWN
};

enum class Function : uint8_t
{
    DRAW_LINE3D,
    DRAW_ARROW,
    PLANE_XY,
    PLANE_XZ,
    PLANE_YZ,
    GRID_ON,
    GRID_OFF,
    PLOT2,
    PLOT3,
    FAST_PLOT2,
    FAST_PLOT3,
    PLOT2_COLLECTION,
    PLOT3_COLLECTION,
    SET_CURRENT_ELEMENT,
    CREATE_NEW_ELEMENT,
    NEW_ELEMENT,
    SCATTER2,
    SCATTER3,
    QUIVER,
    QUIVER3,
    DRAW_LINE_BETWEEN_POINTS_3D,
    POLYGON_FROM_4_POINTS,
    DRAW_TRIANGLES_3D,
    DRAW_TRIANGLE_3D,
    DRAW_TILES,
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
    STAIRS,
    SHOW_LEGEND,
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

enum class ScatterStyleType : uint8_t
{
    SQUARE,
    CIRCLE,
    DISC,
    PLUS,
    CROSS
};

enum class LineStyleType : uint8_t
{
    DASHED,
    DOTTED,
    LONG_DASHED
};

}  // namespace internal

namespace properties
{
constexpr internal::PropertyType PERSISTENT = internal::PropertyType::PERSISTENT;
constexpr internal::PropertyType LINE_STRIP = internal::PropertyType::LINE_STRIP;
}  // namespace properties

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

}  // namespace dvs

#endif // DVS_ENUMERATIONS_H_
