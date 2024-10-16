#ifndef DUOPLOT_ENUMERATIONS_H_
#define DUOPLOT_ENUMERATIONS_H_

#include <stdint.h>

typedef enum S_duoplot_internal_CommunicationHeaderObjectType
{
    DUOPLOT_INTERNAL_CHOT_FUNCTION,
    DUOPLOT_INTERNAL_CHOT_NUM_BUFFERS_REQUIRED,
    DUOPLOT_INTERNAL_CHOT_NUM_BYTES,
    DUOPLOT_INTERNAL_CHOT_DATA_STRUCTURE,
    DUOPLOT_INTERNAL_CHOT_BYTES_PER_ELEMENT,
    DUOPLOT_INTERNAL_CHOT_DATA_TYPE,
    DUOPLOT_INTERNAL_CHOT_NUM_CHANNELS,
    DUOPLOT_INTERNAL_CHOT_NUM_ELEMENTS,
    DUOPLOT_INTERNAL_CHOT_HAS_COLOR,
    DUOPLOT_INTERNAL_CHOT_HAS_POINT_SIZES,
    DUOPLOT_INTERNAL_CHOT_NUM_VERTICES,
    DUOPLOT_INTERNAL_CHOT_NUM_INDICES,
    DUOPLOT_INTERNAL_CHOT_NUM_OBJECTS,
    DUOPLOT_INTERNAL_CHOT_DIMENSION_2D,
    DUOPLOT_INTERNAL_CHOT_HAS_PAYLOAD,
    DUOPLOT_INTERNAL_CHOT_AZIMUTH,
    DUOPLOT_INTERNAL_CHOT_ELEVATION,
    DUOPLOT_INTERNAL_CHOT_AXIS_MIN_MAX_VEC,
    DUOPLOT_INTERNAL_CHOT_VEC3,
    DUOPLOT_INTERNAL_CHOT_SCALE_MATRIX,
    DUOPLOT_INTERNAL_CHOT_TRANSLATION_VECTOR,
    DUOPLOT_INTERNAL_CHOT_ROTATION_MATRIX,
    DUOPLOT_INTERNAL_CHOT_PROJECT_FILE_NAME,
    DUOPLOT_INTERNAL_CHOT_SCREENSHOT_BASE_PATH,
    DUOPLOT_INTERNAL_CHOT_TITLE_STRING,
    DUOPLOT_INTERNAL_CHOT_LABEL,
    DUOPLOT_INTERNAL_CHOT_HANDLE_STRING,
    DUOPLOT_INTERNAL_CHOT_INT32,
    DUOPLOT_INTERNAL_CHOT_POS2D,
    DUOPLOT_INTERNAL_CHOT_FIGURE_NUM,
    DUOPLOT_INTERNAL_CHOT_PARENT_NAME,
    DUOPLOT_INTERNAL_CHOT_PARENT_TYPE,
    DUOPLOT_INTERNAL_CHOT_ELEMENT_NAME,
    DUOPLOT_INTERNAL_CHOT_GUI_ELEMENT_TYPE,
    DUOPLOT_INTERNAL_CHOT_PROPERTY,
    DUOPLOT_INTERNAL_CHOT_ITEM_ID,
    DUOPLOT_INTERNAL_CHOT_NUM_NAMES,
    DUOPLOT_INTERNAL_CHOT_UNKNOWN
} duoplot_internal_CommunicationHeaderObjectType;

typedef enum S_duoplot_internal_PropertyType
{
    DUOPLOT_INTERNAL_PT_LINE_WIDTH,
    DUOPLOT_INTERNAL_PT_ALPHA,
    DUOPLOT_INTERNAL_PT_Z_OFFSET,
    DUOPLOT_INTERNAL_PT_TRANSFORM,
    DUOPLOT_INTERNAL_PT_NAME,
    DUOPLOT_INTERNAL_PT_DISTANCE_FROM,
    DUOPLOT_INTERNAL_PT_LINE_STYLE,
    DUOPLOT_INTERNAL_PT_COLOR,
    DUOPLOT_INTERNAL_PT_EDGE_COLOR,
    DUOPLOT_INTERNAL_PT_FACE_COLOR,
    DUOPLOT_INTERNAL_PT_SILHOUETTE,
    DUOPLOT_INTERNAL_PT_COLOR_MAP,
    DUOPLOT_INTERNAL_PT_POINT_SIZE,
    DUOPLOT_INTERNAL_PT_BUFFER_SIZE,
    DUOPLOT_INTERNAL_PT_SCATTER_STYLE,
    DUOPLOT_INTERNAL_PT_PROPERTY_FLAG,
    DUOPLOT_INTERNAL_PT_ITEM_ID,
    DUOPLOT_INTERNAL_PT_UNKNOWN
} PropertyType;

typedef enum S_duoplot_internal_Function
{
    DUOPLOT_INTERNAL_F_DRAW_LINE3D,
    DUOPLOT_INTERNAL_F_DRAW_ARROW,
    DUOPLOT_INTERNAL_F_PLANE_XY,
    DUOPLOT_INTERNAL_F_PLANE_XZ,
    DUOPLOT_INTERNAL_F_PLANE_YZ,
    DUOPLOT_INTERNAL_F_GRID_ON,
    DUOPLOT_INTERNAL_F_GRID_OFF,
    DUOPLOT_INTERNAL_F_PLOT2,
    DUOPLOT_INTERNAL_F_PLOT3,
    DUOPLOT_INTERNAL_F_REAL_TIME_PLOT,
    DUOPLOT_INTERNAL_F_CUBE,
    DUOPLOT_INTERNAL_F_SPHERE,
    DUOPLOT_INTERNAL_F_PROPERTIES_EXTENSION,
    DUOPLOT_INTERNAL_F_PROPERTIES_EXTENSION_MULTIPLE,
    DUOPLOT_INTERNAL_F_LINE_COLLECTION2,
    DUOPLOT_INTERNAL_F_LINE_COLLECTION3,
    DUOPLOT_INTERNAL_F_STEM,
    DUOPLOT_INTERNAL_F_FAST_PLOT2,
    DUOPLOT_INTERNAL_F_FAST_PLOT3,
    DUOPLOT_INTERNAL_F_PLOT_COLLECTION2,
    DUOPLOT_INTERNAL_F_PLOT_COLLECTION3,
    DUOPLOT_INTERNAL_F_SET_CURRENT_ELEMENT,
    DUOPLOT_INTERNAL_F_CREATE_NEW_ELEMENT,
    DUOPLOT_INTERNAL_F_DELETE_PLOT_OBJECT,
    DUOPLOT_INTERNAL_F_CURRENT_ELEMENT_AS_IMAGE_VIEW,
    DUOPLOT_INTERNAL_F_OPEN_PROJECT_FILE,
    DUOPLOT_INTERNAL_F_NEW_ELEMENT,
    DUOPLOT_INTERNAL_F_WAIT_FOR_FLUSH,
    DUOPLOT_INTERNAL_F_FLUSH_ELEMENT,
    DUOPLOT_INTERNAL_F_FLUSH_MULTIPLE_ELEMENTS,
    DUOPLOT_INTERNAL_F_SCATTER2,
    DUOPLOT_INTERNAL_F_SCATTER3,
    DUOPLOT_INTERNAL_F_QUIVER,
    DUOPLOT_INTERNAL_F_QUIVER3,
    DUOPLOT_INTERNAL_F_DRAW_LINE_BETWEEN_POINTS_3D,
    DUOPLOT_INTERNAL_F_POLYGON_FROM_4_POINTS,
    DUOPLOT_INTERNAL_F_DRAW_TRIANGLES_3D,
    DUOPLOT_INTERNAL_F_DRAW_TRIANGLE_3D,
    DUOPLOT_INTERNAL_F_DRAW_TILES,
    DUOPLOT_INTERNAL_F_DRAW_MESH,
    DUOPLOT_INTERNAL_F_DRAW_MESH_SEPARATE_VECTORS,
    DUOPLOT_INTERNAL_F_HOLD_ON,
    DUOPLOT_INTERNAL_F_HOLD_OFF,
    DUOPLOT_INTERNAL_F_POSITION,
    DUOPLOT_INTERNAL_F_SURF,
    DUOPLOT_INTERNAL_F_IM_SHOW,
    DUOPLOT_INTERNAL_F_DISABLE_AXES_FROM_MIN_MAX,
    DUOPLOT_INTERNAL_F_SET_AXES_BOX_SCALE_FACTOR,
    DUOPLOT_INTERNAL_F_SET_TITLE,
    DUOPLOT_INTERNAL_F_SET_OBJECT_TRANSFORM,
    DUOPLOT_INTERNAL_F_IS_BUSY_RENDERING,
    DUOPLOT_INTERNAL_F_GLOBAL_ILLUMINATION,
    DUOPLOT_INTERNAL_F_AXES_2D,
    DUOPLOT_INTERNAL_F_AXES_3D,
    DUOPLOT_INTERNAL_F_VIEW,
    DUOPLOT_INTERNAL_F_CLEAR,
    DUOPLOT_INTERNAL_F_SOFT_CLEAR,
    DUOPLOT_INTERNAL_F_STAIRS,
    DUOPLOT_INTERNAL_F_SHOW_LEGEND,
    DUOPLOT_INTERNAL_F_DISABLE_SCALE_ON_ROTATION,
    DUOPLOT_INTERNAL_F_AXES_SQUARE,
    DUOPLOT_INTERNAL_F_GET_FLOAT_PARAMETER,
    DUOPLOT_INTERNAL_F_SCREENSHOT,
    DUOPLOT_INTERNAL_F_QUERY_FOR_SYNC_OF_GUI_DATA,
    DUOPLOT_INTERNAL_F_SET_GUI_ELEMENT_LABEL,
    DUOPLOT_INTERNAL_F_SET_GUI_ELEMENT_ENABLED,
    DUOPLOT_INTERNAL_F_SET_GUI_ELEMENT_DISABLED,
    DUOPLOT_INTERNAL_F_SET_GUI_ELEMENT_MIN_VALUE,
    DUOPLOT_INTERNAL_F_SET_GUI_ELEMENT_MAX_VALUE,
    DUOPLOT_INTERNAL_F_SET_GUI_ELEMENT_VALUE,
    DUOPLOT_INTERNAL_F_SET_GUI_ELEMENT_STEP,
    DUOPLOT_INTERNAL_F_UNKNOWN
} duoplot_internal_Function;

typedef enum S_duoplot_internal_DataType
{
    DUOPLOT_INTERNAL_DT_FLOAT,
    DUOPLOT_INTERNAL_DT_DOUBLE,
    DUOPLOT_INTERNAL_DT_INT8,
    DUOPLOT_INTERNAL_DT_INT16,
    DUOPLOT_INTERNAL_DT_INT32,
    DUOPLOT_INTERNAL_DT_INT64,
    DUOPLOT_INTERNAL_DT_UINT8,
    DUOPLOT_INTERNAL_DT_UINT16,
    DUOPLOT_INTERNAL_DT_UINT32,
    DUOPLOT_INTERNAL_DT_UINT64,
    DUOPLOT_INTERNAL_DT_UNKNOWN
} duoplot_internal_DataType;

typedef enum S_duoplot_internal_ColorMapT
{
    DUOPLOT_INTERNAL_CM_JET,
    DUOPLOT_INTERNAL_CM_HSV,
    DUOPLOT_INTERNAL_CM_MAGMA,
    DUOPLOT_INTERNAL_CM_VIRIDIS,
    DUOPLOT_INTERNAL_CM_PASTEL,
    DUOPLOT_INTERNAL_CM_JET_SOFT,
    DUOPLOT_INTERNAL_CM_JET_BRIGHT,
    DUOPLOT_INTERNAL_CM_UNKNOWN
} duoplot_internal_ColorMapT;

typedef enum S_duoplot_internal_DistanceFromType
{
    DUOPLOT_INTERNAL_DFT_X,
    DUOPLOT_INTERNAL_DFT_Y,
    DUOPLOT_INTERNAL_DFT_Z,
    DUOPLOT_INTERNAL_DFT_XY,
    DUOPLOT_INTERNAL_DFT_XZ,
    DUOPLOT_INTERNAL_DFT_YZ,
    DUOPLOT_INTERNAL_DFT_XYZ
} duoplot_internal_DistanceFromType;

typedef enum S_duoplot_PropertyFlag
{
    DUOPLOT_INTERNAL_PF_PERSISTENT,
    DUOPLOT_INTERNAL_PF_APPENDABLE,
    DUOPLOT_INTERNAL_PF_INTERPOLATE_COLORMAP,
    DUOPLOT_INTERNAL_PF_UPDATABLE,
    DUOPLOT_INTERNAL_PF_FAST_PLOT,
    DUOPLOT_INTERNAL_PF_EXCLUDE_FROM_SELECTION,
    DUOPLOT_INTERNAL_PF_SELECTABLE,
    DUOPLOT_INTERNAL_PF_UNKNOWN
} duoplot_PropertyFlag;

typedef enum S_duoplot_internal_GuiElementType
{
    DUOPLOT_INTERNAL_GUI_ET_BUTTON,
    DUOPLOT_INTERNAL_GUI_ET_SLIDER,
    DUOPLOT_INTERNAL_GUI_ET_CHECKBOX,
    DUOPLOT_INTERNAL_GUI_ET_EDITABLE_TEXT,
    DUOPLOT_INTERNAL_GUI_ET_DROPDOWN_MENU,
    DUOPLOT_INTERNAL_GUI_ET_LIST_BOX,
    DUOPLOT_INTERNAL_GUI_ET_RADIO_BUTTON_GROUP,
    DUOPLOT_INTERNAL_GUI_ET_TEXT_LABEL,
    DUOPLOT_INTERNAL_GUI_ET_STATIC_BOX,
    DUOPLOT_INTERNAL_GUI_ET_PLOT_PANE,
    DUOPLOT_INTERNAL_GUI_ET_SCROLLING_TEXT,
    DUOPLOT_INTERNAL_GUI_ET_UNKNOWN
} duoplot_internal_GuiElementType;

#endif  // DUOPLOT_ENUMERATIONS_H_
