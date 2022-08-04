#ifndef DVS_ENUMERATIONS_H_
#define DVS_ENUMERATIONS_H_

#include <stdint.h>

typedef enum S_FunctionHeaderObjectType
{
    FHOT_FUNCTION,
    FHOT_NUM_BUFFERS_REQUIRED,
    FHOT_NUM_BYTES,
    FHOT_DATA_STRUCTURE,
    FHOT_BYTES_PER_ELEMENT,
    FHOT_DATA_TYPE,
    FHOT_NUM_CHANNELS,
    FHOT_NUM_ELEMENTS,
    FHOT_NUM_VERTICES,
    FHOT_NUM_INDICES,
    FHOT_DIMENSION_2D,
    FHOT_HAS_PAYLOAD,
    FHOT_AZIMUTH,
    FHOT_ELEVATION,
    FHOT_AXIS_MIN_MAX_VEC,
    FHOT_POS2D,
    FHOT_FIGURE_NUM,
    FHOT_PARENT_NAME,
    FHOT_PARENT_TYPE,
    FHOT_ELEMENT_NAME,
    FHOT_GUI_ELEMENT_TYPE,
    FHOT_PROPERTY,
    FHOT_UNKNOWN
} FunctionHeaderObjectType;

typedef enum S_PropertyType
{
    PT_LINE_WIDTH,
    PT_ALPHA,
    PT_NAME,
    PT_LINE_STYLE,
    PT_COLOR,
    PT_EDGE_COLOR,
    PT_FACE_COLOR,
    PT_COLOR_MAP,
    PT_POINT_SIZE,
    PT_PERSISTENT,
    PT_UNKNOWN
} PropertyType;

typedef enum S_Function
{
    F_DRAW_LINE3D,
    F_PLANE_XY,
    F_PLANE_XZ,
    F_PLANE_YZ,
    F_GRID_ON,
    F_GRID_OFF,
    F_PLOT2,
    F_PLOT3,
    F_SET_CURRENT_ELEMENT,
    F_CREATE_NEW_ELEMENT,
    F_NEW_ELEMENT,
    F_SCATTER2,
    F_SCATTER3,
    F_DRAW_LINE_BETWEEN_POINTS_3D,
    F_POLYGON_FROM_4_POINTS,
    F_DRAW_TRIANGLES_3D,
    F_DRAW_TRIANGLE_3D,
    F_DRAW_MESH,
    F_HOLD_ON,
    F_HOLD_OFF,
    F_POSITION,
    F_SURF,
    F_IM_SHOW,
    F_AXES_2D,
    F_AXES_3D,
    F_VIEW,
    F_CLEAR,
    F_SOFT_CLEAR,
    F_UNKNOWN
} Function;

typedef enum S_DataType
{
    DT_FLOAT,
    DT_DOUBLE,
    DT_INT8,
    DT_INT16,
    DT_INT32,
    DT_INT64,
    DT_UINT8,
    DT_UINT16,
    DT_UINT32,
    DT_UINT64,
    DT_UNKNOWN
} DataType;

typedef enum S_DataStructure
{
    DS_MATRIX,
    DS_VECTOR,
    DS_IMAGE,
    DS_TEXT,
    DS_UNKNOWN
} DataStructure;

typedef enum S_ColorMapType
{
    CMT_JET,
    CMT_RAINBOW,
    CMT_MAGMA,
    CMT_VIRIDIS,
    CMT_UNKNOWN
} ColorMapType;

typedef enum S_ElementType
{
    ET_PLOT,
    ET_IMAGE,
    ET_WORLD_3D,
    ET_BUTTON,
    ET_TEXT_FIELD,
    ET_UNKNOWN
} ElementType;

typedef enum S_ElementParent
{
    EP_TAB,
    EP_WINDOW,
    EP_UNKNOWN
} ElementParent;

/*namespace properties
{
constexpr internal::PropertyType PERSISTENT = internal::PropertyType::PERSISTENT;
}*/

#endif // DVS_ENUMERATIONS_H_
