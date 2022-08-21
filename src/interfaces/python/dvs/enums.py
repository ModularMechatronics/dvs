from enum import Enum
import numpy as np


class FunctionHeaderObjectType(Enum):
    FUNCTION = 0
    NUM_BUFFERS_REQUIRED = 1
    NUM_BYTES = 2
    DATA_STRUCTURE = 3
    BYTES_PER_ELEMENT = 4
    DATA_TYPE = 5
    NUM_CHANNELS = 6
    NUM_ELEMENTS = 7
    NUM_VERTICES = 8
    NUM_INDICES = 9
    DIMENSION_2D = 10
    HAS_PAYLOAD = 11
    AZIMUTH = 12
    ELEVATION = 13
    AXIS_MIN_MAX_VEC = 14
    POS2D = 15
    FIGURE_NUM = 16
    PARENT_NAME = 17
    PARENT_TYPE = 18
    ELEMENT_NAME = 19
    GUI_ELEMENT_TYPE = 20
    PROPERTY = 21
    UNKNOWN = 22
    _TRANSMISSION_DATA_TYPE = np.uint16


class PropertyType(Enum):
    LINE_WIDTH = 0
    ALPHA = 1
    NAME = 2
    LINE_STYLE = 3
    COLOR = 4
    EDGE_COLOR = 5
    FACE_COLOR = 6
    COLOR_MAP = 7
    POINT_SIZE = 8
    PERSISTENT = 9
    UNKNOWN = 10
    _TRANSMISSION_DATA_TYPE = np.uint8


class Function(Enum):
    DRAW_LINE3D = 0
    PLANE_XY = 1
    PLANE_XZ = 2
    PLANE_YZ = 3
    GRID_ON = 4
    GRID_OFF = 5
    PLOT2 = 6
    PLOT3 = 7
    SET_CURRENT_ELEMENT = 8
    CREATE_NEW_ELEMENT = 9
    NEW_ELEMENT = 10
    SCATTER2 = 11
    SCATTER3 = 12
    DRAW_LINE_BETWEEN_POINTS_3D = 13
    POLYGON_FROM_4_POINTS = 14
    DRAW_TRIANGLES_3D = 15
    DRAW_TRIANGLE_3D = 16
    DRAW_MESH = 17
    HOLD_ON = 18
    HOLD_OFF = 19
    POSITION = 20
    SURF = 21
    IM_SHOW = 22
    AXES_2D = 23
    AXES_3D = 24
    VIEW = 25
    CLEAR = 26
    SOFT_CLEAR = 27
    UNKNOWN = 28
    _TRANSMISSION_DATA_TYPE = np.uint8


class DataType(Enum):
    FLOAT = 0
    DOUBLE = 1
    INT8 = 2
    INT16 = 3
    INT32 = 4
    INT64 = 5
    UINT8 = 6
    UINT16 = 7
    UINT32 = 8
    UINT64 = 9
    UNKNOWN = 10
    _TRANSMISSION_DATA_TYPE = np.uint8


class DataStructure(Enum):
    MATRIX = 0
    VECTOR = 1
    IMAGE = 2
    TEXT = 3
    UNKNOWN = 4
    _TRANSMISSION_DATA_TYPE = np.uint8


class ColorMapType(Enum):
    JET = 0
    RAINBOW = 1
    MAGMA = 2
    VIRIDIS = 3
    UNKNOWN = 4
    _TRANSMISSION_DATA_TYPE = np.uint8


class ElementType(Enum):
    PLOT = 0
    IMAGE = 1
    WORLD_3D = 2
    BUTTON = 3
    TEXT_FIELD = 4
    UNKNOWN = 5
    _TRANSMISSION_DATA_TYPE = np.uint8


class ElementParent(Enum):
    TAB = 0
    WINDOW = 1
    UNKNOWN = 2
    _TRANSMISSION_DATA_TYPE = np.uint8
