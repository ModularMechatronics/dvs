#ifndef DVS_ENUMERATIONS_H_
#define DVS_ENUMERATIONS_H_

#include <stdint.h>

#include "dvs/item_id.h"

namespace dvs
{
namespace internal
{
enum class CommunicationHeaderObjectType : uint16_t
{
    FUNCTION,
    NUM_BUFFERS_REQUIRED,
    NUM_BYTES,
    DATA_STRUCTURE,
    BYTES_PER_ELEMENT,
    DATA_TYPE,
    NUM_CHANNELS,
    NUM_ELEMENTS,
    HAS_COLOR,
    NUM_VERTICES,
    NUM_INDICES,
    NUM_OBJECTS,
    DIMENSION_2D,
    HAS_PAYLOAD,
    AZIMUTH,
    ELEVATION,
    AXIS_MIN_MAX_VEC,
    VEC3,
    SCALE_MATRIX,
    TRANSLATION_VECTOR,
    ROTATION_MATRIX,
    PROJECT_FILE_NAME,
    POS2D,
    FIGURE_NUM,
    PARENT_NAME,
    PARENT_TYPE,
    ELEMENT_NAME,
    GUI_ELEMENT_TYPE,
    PROPERTY,
    ITEM_ID,
    NUM_NAMES,
    UNKNOWN
};

enum class PropertyFlag : uint8_t
{
    PERSISTENT,
    INTERPOLATE_COLORMAP,
    UPDATABLE,
    UNKNOWN
};

enum class PropertyType : uint8_t
{
    LINE_WIDTH,
    ALPHA,
    Z_OFFSET,
    TRANSFORM,
    NAME,
    DISTANCE_FROM,
    LINE_STYLE,
    COLOR,
    EDGE_COLOR,
    FACE_COLOR,
    COLOR_MAP,
    POINT_SIZE,
    BUFFER_SIZE,
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
    REAL_TIME_PLOT,
    CUBE,
    SPHERE,
    PROPERTIES_EXTENSION,
    LINE_COLLECTION2,
    LINE_COLLECTION3,
    STEM,
    FAST_PLOT2,
    FAST_PLOT3,
    PLOT_COLLECTION2,
    PLOT_COLLECTION3,
    SET_CURRENT_ELEMENT,
    CREATE_NEW_ELEMENT,
    OPEN_PROJECT_FILE,
    NEW_ELEMENT,
    WAIT_FOR_FLUSH,
    FLUSH_ELEMENT,
    FLUSH_MULTIPLE_ELEMENTS,
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
    DRAW_MESH_SEPARATE_VECTORS,
    HOLD_ON,
    HOLD_OFF,
    POSITION,
    SURF,
    IM_SHOW,
    DISABLE_AXES_FROM_MIN_MAX,
    SET_AXES_BOX_SCALE_FACTOR,
    SET_OBJECT_TRANSFORM,
    IS_BUSY_RENDERING,
    GLOBAL_ILLUMINATION,
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

enum class ColorT : uint8_t
{
    RED,
    GREEN,
    BLUE,
    CYAN,
    MAGENTA,
    YELLOW,
    BLACK,
    WHITE,
    GRAY
};

enum class FaceColorT : uint8_t
{
    RED,
    GREEN,
    BLUE,
    CYAN,
    MAGENTA,
    YELLOW,
    BLACK,
    WHITE,
    GRAY,
    NONE
};

enum class EdgeColorT : uint8_t
{
    RED,
    GREEN,
    BLUE,
    CYAN,
    MAGENTA,
    YELLOW,
    BLACK,
    WHITE,
    GRAY,
    NONE
};

}  // namespace internal

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

enum DistanceFromType : uint8_t
{
    X,
    Y,
    Z,
    XY,
    XZ,
    YZ,
    XYZ
};

}  // namespace dvs

inline std::ostream& operator<<(std::ostream& os, const dvs::internal::Function& f)  // TODO: Remove
{
    std::string s = "";

    switch (f)
    {
        case dvs::internal::Function::DRAW_LINE3D:
            s = "DRAW_LINE3D";
            break;
        case dvs::internal::Function::DRAW_ARROW:
            s = "DRAW_ARROW";
            break;
        case dvs::internal::Function::PLANE_XY:
            s = "PLANE_XY";
            break;
        case dvs::internal::Function::PLANE_XZ:
            s = "PLANE_XZ";
            break;
        case dvs::internal::Function::PLANE_YZ:
            s = "PLANE_YZ";
            break;
        case dvs::internal::Function::GRID_ON:
            s = "GRID_ON";
            break;
        case dvs::internal::Function::GRID_OFF:
            s = "GRID_OFF";
            break;
        case dvs::internal::Function::PLOT2:
            s = "PLOT2";
            break;
        case dvs::internal::Function::PLOT3:
            s = "PLOT3";
            break;
        case dvs::internal::Function::REAL_TIME_PLOT:
            s = "REAL_TIME_PLOT";
            break;
        case dvs::internal::Function::CUBE:
            s = "CUBE";
            break;
        case dvs::internal::Function::SPHERE:
            s = "SPHERE";
            break;
        case dvs::internal::Function::PROPERTIES_EXTENSION:
            s = "PROPERTIES_EXTENSION";
            break;
        case dvs::internal::Function::LINE_COLLECTION2:
            s = "LINE_COLLECTION2";
            break;
        case dvs::internal::Function::LINE_COLLECTION3:
            s = "LINE_COLLECTION3";
            break;
        case dvs::internal::Function::STEM:
            s = "STEM";
            break;
        case dvs::internal::Function::FAST_PLOT2:
            s = "FAST_PLOT2";
            break;
        case dvs::internal::Function::FAST_PLOT3:
            s = "FAST_PLOT3";
            break;
        case dvs::internal::Function::PLOT_COLLECTION2:
            s = "PLOT_COLLECTION2";
            break;
        case dvs::internal::Function::PLOT_COLLECTION3:
            s = "PLOT_COLLECTION3";
            break;
        case dvs::internal::Function::SET_CURRENT_ELEMENT:
            s = "SET_CURRENT_ELEMENT";
            break;
        case dvs::internal::Function::CREATE_NEW_ELEMENT:
            s = "CREATE_NEW_ELEMENT";
            break;
        case dvs::internal::Function::OPEN_PROJECT_FILE:
            s = "OPEN_PROJECT_FILE";
            break;
        case dvs::internal::Function::NEW_ELEMENT:
            s = "NEW_ELEMENT";
            break;
        case dvs::internal::Function::WAIT_FOR_FLUSH:
            s = "WAIT_FOR_FLUSH";
            break;
        case dvs::internal::Function::FLUSH_ELEMENT:
            s = "FLUSH_ELEMENT";
            break;
        case dvs::internal::Function::FLUSH_MULTIPLE_ELEMENTS:
            s = "FLUSH_MULTIPLE_ELEMENTS";
            break;
        case dvs::internal::Function::SCATTER2:
            s = "SCATTER2";
            break;
        case dvs::internal::Function::SCATTER3:
            s = "SCATTER3";
            break;
        case dvs::internal::Function::QUIVER:
            s = "QUIVER";
            break;
        case dvs::internal::Function::QUIVER3:
            s = "QUIVER3";
            break;
        case dvs::internal::Function::DRAW_LINE_BETWEEN_POINTS_3D:
            s = "DRAW_LINE_BETWEEN_POINTS_3D";
            break;
        case dvs::internal::Function::POLYGON_FROM_4_POINTS:
            s = "POLYGON_FROM_4_POINTS";
            break;
        case dvs::internal::Function::DRAW_TRIANGLES_3D:
            s = "DRAW_TRIANGLES_3D";
            break;
        case dvs::internal::Function::DRAW_TRIANGLE_3D:
            s = "DRAW_TRIANGLE_3D";
            break;
        case dvs::internal::Function::DRAW_TILES:
            s = "DRAW_TILES";
            break;
        case dvs::internal::Function::DRAW_MESH:
            s = "DRAW_MESH";
            break;
        case dvs::internal::Function::DRAW_MESH_SEPARATE_VECTORS:
            s = "DRAW_MESH_SEPARATE_VECTORS";
            break;
        case dvs::internal::Function::HOLD_ON:
            s = "HOLD_ON";
            break;
        case dvs::internal::Function::HOLD_OFF:
            s = "HOLD_OFF";
            break;
        case dvs::internal::Function::POSITION:
            s = "POSITION";
            break;
        case dvs::internal::Function::SURF:
            s = "SURF";
            break;
        case dvs::internal::Function::IM_SHOW:
            s = "IM_SHOW";
            break;
        case dvs::internal::Function::DISABLE_AXES_FROM_MIN_MAX:
            s = "DISABLE_AXES_FROM_MIN_MAX";
            break;
        case dvs::internal::Function::SET_AXES_BOX_SCALE_FACTOR:
            s = "SET_AXES_BOX_SCALE_FACTOR";
            break;
        case dvs::internal::Function::SET_OBJECT_TRANSFORM:
            s = "SET_OBJECT_TRANSFORM";
            break;
        case dvs::internal::Function::IS_BUSY_RENDERING:
            s = "IS_BUSY_RENDERING";
            break;
        case dvs::internal::Function::GLOBAL_ILLUMINATION:
            s = "GLOBAL_ILLUMINATION";
            break;
        case dvs::internal::Function::AXES_2D:
            s = "AXES_2D";
            break;
        case dvs::internal::Function::AXES_3D:
            s = "AXES_3D";
            break;
        case dvs::internal::Function::VIEW:
            s = "VIEW";
            break;
        case dvs::internal::Function::CLEAR:
            s = "CLEAR";
            break;
        case dvs::internal::Function::SOFT_CLEAR:
            s = "SOFT_CLEAR";
            break;
        case dvs::internal::Function::STAIRS:
            s = "STAIRS";
            break;
        case dvs::internal::Function::SHOW_LEGEND:
            s = "SHOW_LEGEND";
            break;
        case dvs::internal::Function::UNKNOWN:
            s = "UNKNOWN";
            break;
        default:
            s = "default";
            break;
    }
    os << s;

    return os;
}

#endif  // DVS_ENUMERATIONS_H_
