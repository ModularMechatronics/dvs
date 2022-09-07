import sys
import numpy as np

from enums import *
from internal_types import *

NUM_BYTES_FOR_NAME = 23


def serialize_color(col: Color):
    return PropertyType.COLOR.value.to_bytes(1, sys.byteorder) + col.r.to_bytes(1, sys.byteorder) + \
        col.g.to_bytes(1, sys.byteorder) + col.b.to_bytes(1, sys.byteorder)


def serialize_one_byte_num(num):
    return np.uint8(num).tobytes()


def serialize_two_byte_num(num):
    return np.uint16(num).tobytes()


def serialize_str(name):
    return bytearray(name.encode('utf8'))


def serialize_name(name: str):
    return PropertyType.NAME.value.to_bytes(1, sys.byteorder) + np.uint64(NUM_BYTES_FOR_NAME).tobytes() + bytearray(name.encode('utf8'))


def serialize_color_map(cm):
    return np.uint8(ColorMapType.MAGMA.value).tobytes()


def serialize_axes(axis_vecs):
    return np.array([np.float64(a) for a in (axis_vecs[0] + axis_vecs[1])]).tobytes()


PROPERTY_SERIALIZATION_FUNCTIONS = {PropertyType.COLOR: serialize_color,
                                    PropertyType.EDGE_COLOR: serialize_color,
                                    PropertyType.FACE_COLOR: serialize_color,
                                    PropertyType.COLOR_MAP: serialize_color_map,
                                    PropertyType.LINE_WIDTH: serialize_one_byte_num,
                                    PropertyType.POINT_SIZE: serialize_one_byte_num,
                                    PropertyType.LINE_STYLE: serialize_str,
                                    PropertyType.ALPHA: serialize_one_byte_num,
                                    PropertyType.NAME: serialize_name}

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
    TransmissionHeaderObjectType.FUNCTION: lambda x: np.uint8(x.value).tobytes(),
    TransmissionHeaderObjectType.NUM_BUFFERS_REQUIRED: None,
    TransmissionHeaderObjectType.NUM_BYTES: None,
    TransmissionHeaderObjectType.DATA_STRUCTURE: None,
    TransmissionHeaderObjectType.BYTES_PER_ELEMENT: lambda x: np.uint8(x).tobytes(),
    TransmissionHeaderObjectType.DATA_TYPE: lambda x: np.uint8(x.value).tobytes(),
    TransmissionHeaderObjectType.NUM_ELEMENTS: lambda x: np.uint32(x).tobytes(),
    TransmissionHeaderObjectType.NUM_VERTICES: lambda x: np.uint32(x).tobytes(),
    TransmissionHeaderObjectType.NUM_INDICES: lambda x: np.uint32(x).tobytes(),
    TransmissionHeaderObjectType.DIMENSION_2D: lambda x: np.uint64(x).tobytes(),
    TransmissionHeaderObjectType.HAS_PAYLOAD: None,
    TransmissionHeaderObjectType.AZIMUTH: lambda x: np.float32(x).tobytes(),
    TransmissionHeaderObjectType.ELEVATION: lambda x: np.float32(x).tobytes(),
    TransmissionHeaderObjectType.AXIS_MIN_MAX_VEC: serialize_axes,
    TransmissionHeaderObjectType.POS2D: None,
    TransmissionHeaderObjectType.FIGURE_NUM: None,
    TransmissionHeaderObjectType.PARENT_NAME: None,
    TransmissionHeaderObjectType.PARENT_TYPE: None,
    TransmissionHeaderObjectType.ELEMENT_NAME: serialize_name,
    TransmissionHeaderObjectType.GUI_ELEMENT_TYPE: None,
    TransmissionHeaderObjectType.PROPERTY: None,
    TransmissionHeaderObjectType.NUM_CHANNELS: lambda x: np.uint8(x).tobytes(),
    TransmissionHeaderObjectType.UNKNOWN: None
}


SIZE_OF_FUNCTION_HEADER_OBJECT = {TransmissionHeaderObjectType.FUNCTION: 1,
                                  TransmissionHeaderObjectType.NUM_BUFFERS_REQUIRED: None,
                                  TransmissionHeaderObjectType.NUM_BYTES: 4,
                                  TransmissionHeaderObjectType.DATA_STRUCTURE: 1,
                                  TransmissionHeaderObjectType.BYTES_PER_ELEMENT: 1,
                                  TransmissionHeaderObjectType.DATA_TYPE: 1,
                                  TransmissionHeaderObjectType.NUM_ELEMENTS: 4,
                                  TransmissionHeaderObjectType.NUM_VERTICES: 4,
                                  TransmissionHeaderObjectType.NUM_INDICES: 4,
                                  TransmissionHeaderObjectType.DIMENSION_2D: 2 * 8,
                                  TransmissionHeaderObjectType.HAS_PAYLOAD: None,
                                  TransmissionHeaderObjectType.AZIMUTH: 4,
                                  TransmissionHeaderObjectType.ELEVATION: 4,
                                  TransmissionHeaderObjectType.AXIS_MIN_MAX_VEC: 8 * 3 * 2,
                                  TransmissionHeaderObjectType.POS2D: None,
                                  TransmissionHeaderObjectType.FIGURE_NUM: None,
                                  TransmissionHeaderObjectType.PARENT_NAME: None,
                                  TransmissionHeaderObjectType.PARENT_TYPE: None,
                                  TransmissionHeaderObjectType.ELEMENT_NAME: NUM_BYTES_FOR_NAME,
                                  TransmissionHeaderObjectType.GUI_ELEMENT_TYPE: None,
                                  TransmissionHeaderObjectType.PROPERTY: 1,
                                  TransmissionHeaderObjectType.NUM_CHANNELS: 1,
                                  TransmissionHeaderObjectType.UNKNOWN: None}
