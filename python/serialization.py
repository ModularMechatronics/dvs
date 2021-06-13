import sys
import numpy as np

from enums import *
from internal_types import *

def serialize_color(col: Color):
    return col.r.to_bytes(1, sys.byteorder) + col.g.to_bytes(1, sys.byteorder) + col.b.to_bytes(1, sys.byteorder)

def serialize_one_byte_num(num):
    return np.uint8(num).tobytes()

def serialize_two_byte_num(num):
    return np.uint16(num).tobytes()

def serialize_str(name):
    return bytearray(name.encode('utf8'))

def serialize_color_map(cm):
    return np.uint8(ColorMapType.MAGMA.value).tobytes()


PROPERTY_SERIALIZATION_FUNCTIONS = {PropertyType.COLOR: serialize_color,
                                    PropertyType.EDGE_COLOR: serialize_color,
                                    PropertyType.FACE_COLOR: serialize_color,
                                    PropertyType.COLOR_MAP: serialize_color_map,
                                    PropertyType.LINE_WIDTH: serialize_one_byte_num,
                                    PropertyType.POINT_SIZE: serialize_one_byte_num,
                                    PropertyType.LINE_STYLE: serialize_str,
                                    PropertyType.ALPHA: serialize_one_byte_num,
                                    PropertyType.NAME: serialize_str}

SIZE_OF_PROPERTY = {
    PropertyType.COLOR: 3,
    PropertyType.EDGE_COLOR: 3,
    PropertyType.FACE_COLOR: 3,
    PropertyType.COLOR_MAP: 1,
    PropertyType.LINE_WIDTH: 1,
    PropertyType.POINT_SIZE: 1,
    PropertyType.LINE_STYLE: 10,
    PropertyType.ALPHA: 1,
    PropertyType.NAME: 10
}


FUNCTION_HEADER_OBJECT_SERIALIZATION_FUNCTION = {
    FunctionHeaderObjectType.FUNCTION: lambda x: np.uint8(x.value).tobytes(),
    FunctionHeaderObjectType.NUM_BUFFERS_REQUIRED: None,
    FunctionHeaderObjectType.NUM_BYTES: None,
    FunctionHeaderObjectType.DATA_STRUCTURE: None,
    FunctionHeaderObjectType.BYTES_PER_ELEMENT: lambda x: np.uint8(x).tobytes(),
    FunctionHeaderObjectType.DATA_TYPE: lambda x: np.uint8(x.value).tobytes(),
    FunctionHeaderObjectType.NUM_ELEMENTS: lambda x: np.uint32(x).tobytes(),
    FunctionHeaderObjectType.NUM_VERTICES: lambda x: np.uint32(x).tobytes(),
    FunctionHeaderObjectType.NUM_INDICES: lambda x: np.uint32(x).tobytes(),
    FunctionHeaderObjectType.DIMENSION_2D: None,
    FunctionHeaderObjectType.HAS_PAYLOAD: None,
    FunctionHeaderObjectType.AZIMUTH: None,
    FunctionHeaderObjectType.ELEVATION: None,
    FunctionHeaderObjectType.AXIS_MIN_MAX_VEC: None,
    FunctionHeaderObjectType.POS2D: None,
    FunctionHeaderObjectType.FIGURE_NUM: None,
    FunctionHeaderObjectType.PARENT_NAME: None,
    FunctionHeaderObjectType.PARENT_TYPE: None,
    FunctionHeaderObjectType.ELEMENT_NAME: None,
    FunctionHeaderObjectType.GUI_ELEMENT_TYPE: None,
    FunctionHeaderObjectType.PROPERTY: None,
    FunctionHeaderObjectType.UNKNOWN: None
}


SIZE_OF_FUNCTION_HEADER_OBJECT = {FunctionHeaderObjectType.FUNCTION: 1,
                                  FunctionHeaderObjectType.NUM_BUFFERS_REQUIRED: None,
                                  FunctionHeaderObjectType.NUM_BYTES: 4,
                                  FunctionHeaderObjectType.DATA_STRUCTURE: 1,
                                  FunctionHeaderObjectType.BYTES_PER_ELEMENT: 1,
                                  FunctionHeaderObjectType.DATA_TYPE: 1,
                                  FunctionHeaderObjectType.NUM_ELEMENTS: 4,
                                  FunctionHeaderObjectType.NUM_VERTICES: 4,
                                  FunctionHeaderObjectType.NUM_INDICES: 4,
                                  FunctionHeaderObjectType.DIMENSION_2D: None,
                                  FunctionHeaderObjectType.HAS_PAYLOAD: None,
                                  FunctionHeaderObjectType.AZIMUTH: None,
                                  FunctionHeaderObjectType.ELEVATION: None,
                                  FunctionHeaderObjectType.AXIS_MIN_MAX_VEC: None,
                                  FunctionHeaderObjectType.POS2D: None,
                                  FunctionHeaderObjectType.FIGURE_NUM: None,
                                  FunctionHeaderObjectType.PARENT_NAME: None,
                                  FunctionHeaderObjectType.PARENT_TYPE: None,
                                  FunctionHeaderObjectType.ELEMENT_NAME: None,
                                  FunctionHeaderObjectType.GUI_ELEMENT_TYPE: None,
                                  FunctionHeaderObjectType.PROPERTY: None,
                                  FunctionHeaderObjectType.UNKNOWN: None}
