import sys
import numpy as np

from enums import *
from properties import *

NUM_BYTES_FOR_NAME = 23


def serialize_color(col: Color):
    return (
        PropertyType.COLOR.value.to_bytes(1, sys.byteorder)
        + col.r.to_bytes(1, sys.byteorder)
        + col.g.to_bytes(1, sys.byteorder)
        + col.b.to_bytes(1, sys.byteorder)
    )


def serialize_one_byte_num(num):
    return np.uint8(num).tobytes()


def serialize_two_byte_num(num):
    return np.uint16(num).tobytes()


def serialize_str(name: str):
    return bytearray(name.encode("utf8"))


def serialize_name(name: str):
    return (
        PropertyType.NAME.value.to_bytes(1, sys.byteorder)
        + np.uint64(NUM_BYTES_FOR_NAME).tobytes()
        + bytearray(name.encode("utf8"))
    )


def serialize_color_map(cm):
    return np.uint8(cm.value).tobytes()


def serialize_axes(axis_vecs):
    return np.array(
        [
            np.float64(a)
            for a in (
                axis_vecs[0].x,
                axis_vecs[0].y,
                axis_vecs[0].z,
                axis_vecs[1].x,
                axis_vecs[1].y,
                axis_vecs[1].z,
            )
        ]
    ).tobytes()


PROPERTY_SERIALIZATION_FUNCTIONS = {
    PropertyType.COLOR: serialize_color,
    PropertyType.EDGE_COLOR: serialize_color,
    PropertyType.FACE_COLOR: serialize_color,
    PropertyType.COLOR_MAP: serialize_color_map,
    PropertyType.LINE_WIDTH: serialize_one_byte_num,
    PropertyType.POINT_SIZE: serialize_one_byte_num,
    PropertyType.LINE_STYLE: serialize_str,
    PropertyType.ALPHA: serialize_one_byte_num,
    PropertyType.NAME: serialize_name,
}

SIZE_OF_PROPERTY = {
    PropertyType.COLOR: 3,
    PropertyType.EDGE_COLOR: 3,
    PropertyType.FACE_COLOR: 3,
    PropertyType.COLOR_MAP: 2,
    PropertyType.LINE_WIDTH: 1,
    PropertyType.POINT_SIZE: 1,
    PropertyType.LINE_STYLE: 10,
    PropertyType.ALPHA: 1,
    PropertyType.NAME: 10,
}

FUNCTION_HEADER_OBJECT_SERIALIZATION_FUNCTION = {
    CommunicationHeaderObjectType.FUNCTION: None,
    CommunicationHeaderObjectType.NUM_BUFFERS_REQUIRED: None,
    CommunicationHeaderObjectType.NUM_BYTES: None,
    CommunicationHeaderObjectType.DATA_STRUCTURE: None,
    CommunicationHeaderObjectType.BYTES_PER_ELEMENT: None,
    CommunicationHeaderObjectType.DATA_TYPE: lambda x: np.uint8(x.value).tobytes(),
    CommunicationHeaderObjectType.NUM_CHANNELS: None,
    CommunicationHeaderObjectType.NUM_ELEMENTS: lambda x: np.uint32(x).tobytes(),
    CommunicationHeaderObjectType.HAS_COLOR: None,
    CommunicationHeaderObjectType.HAS_POINT_SIZES: None,
    CommunicationHeaderObjectType.NUM_VERTICES: None,
    CommunicationHeaderObjectType.NUM_INDICES: None,
    CommunicationHeaderObjectType.NUM_OBJECTS: None,
    CommunicationHeaderObjectType.DIMENSION_2D: lambda x: np.array(
        [np.uint32(a) for a in x]
    ).tobytes(),
    CommunicationHeaderObjectType.HAS_PAYLOAD: None,
    CommunicationHeaderObjectType.AZIMUTH: lambda x: np.float32(x).tobytes(),
    CommunicationHeaderObjectType.ELEVATION: lambda x: np.float32(x).tobytes(),
    CommunicationHeaderObjectType.AXIS_MIN_MAX_VEC: serialize_axes,
    CommunicationHeaderObjectType.VEC3: None,
    CommunicationHeaderObjectType.SCALE_MATRIX: None,
    CommunicationHeaderObjectType.TRANSLATION_VECTOR: None,
    CommunicationHeaderObjectType.ROTATION_MATRIX: None,
    CommunicationHeaderObjectType.PROJECT_FILE_NAME: None,
    CommunicationHeaderObjectType.TITLE_STRING: None,
    CommunicationHeaderObjectType.POS2D: None,
    CommunicationHeaderObjectType.FIGURE_NUM: None,
    CommunicationHeaderObjectType.PARENT_NAME: None,
    CommunicationHeaderObjectType.PARENT_TYPE: None,
    CommunicationHeaderObjectType.ELEMENT_NAME: lambda x: PropertyType.NAME.value.to_bytes(
        1, sys.byteorder
    )
    + len(x).to_bytes(1, sys.byteorder)
    + x.encode("utf-8"),
    CommunicationHeaderObjectType.GUI_ELEMENT_TYPE: None,
    CommunicationHeaderObjectType.PROPERTY: None,
    CommunicationHeaderObjectType.ITEM_ID: None,
    CommunicationHeaderObjectType.NUM_NAMES: None,
    CommunicationHeaderObjectType.UNKNOWN: None,
}

"""FUNCTION_HEADER_OBJECT_SERIALIZATION_FUNCTION = {
    CommunicationHeaderObjectType.FUNCTION: lambda x: np.uint8(x.value).tobytes(),
    CommunicationHeaderObjectType.NUM_BUFFERS_REQUIRED: None,
    CommunicationHeaderObjectType.NUM_BYTES: None,
    CommunicationHeaderObjectType.DATA_STRUCTURE: None,
    CommunicationHeaderObjectType.BYTES_PER_ELEMENT: lambda x: np.uint8(x).tobytes(),
    CommunicationHeaderObjectType.DATA_TYPE: lambda x: np.uint8(x.value).tobytes(),
    CommunicationHeaderObjectType.NUM_ELEMENTS: lambda x: np.uint32(x).tobytes(),
    CommunicationHeaderObjectType.NUM_VERTICES: lambda x: np.uint32(x).tobytes(),
    CommunicationHeaderObjectType.NUM_INDICES: lambda x: np.uint32(x).tobytes(),
    CommunicationHeaderObjectType.DIMENSION_2D: lambda x: np.uint64(x).tobytes(),
    CommunicationHeaderObjectType.HAS_PAYLOAD: None,
    CommunicationHeaderObjectType.AZIMUTH: lambda x: np.float32(x).tobytes(),
    CommunicationHeaderObjectType.ELEVATION: lambda x: np.float32(x).tobytes(),
    CommunicationHeaderObjectType.AXIS_MIN_MAX_VEC: serialize_axes,
    CommunicationHeaderObjectType.POS2D: None,
    CommunicationHeaderObjectType.FIGURE_NUM: None,
    CommunicationHeaderObjectType.PARENT_NAME: None,
    CommunicationHeaderObjectType.PARENT_TYPE: None,
    CommunicationHeaderObjectType.ELEMENT_NAME: serialize_name,
    CommunicationHeaderObjectType.GUI_ELEMENT_TYPE: None,
    CommunicationHeaderObjectType.PROPERTY: None,
    CommunicationHeaderObjectType.NUM_CHANNELS: lambda x: np.uint8(x).tobytes(),
    CommunicationHeaderObjectType.UNKNOWN: None
}"""


SIZE_OF_FUNCTION_HEADER_OBJECT = {
    CommunicationHeaderObjectType.FUNCTION: None,
    CommunicationHeaderObjectType.NUM_BUFFERS_REQUIRED: None,
    CommunicationHeaderObjectType.NUM_BYTES: None,
    CommunicationHeaderObjectType.DATA_STRUCTURE: None,
    CommunicationHeaderObjectType.BYTES_PER_ELEMENT: None,
    CommunicationHeaderObjectType.DATA_TYPE: 1,  # uint8
    CommunicationHeaderObjectType.NUM_CHANNELS: None,
    CommunicationHeaderObjectType.NUM_ELEMENTS: 4,  # uint32
    CommunicationHeaderObjectType.HAS_COLOR: None,
    CommunicationHeaderObjectType.HAS_POINT_SIZES: None,
    CommunicationHeaderObjectType.NUM_VERTICES: None,
    CommunicationHeaderObjectType.NUM_INDICES: None,
    CommunicationHeaderObjectType.NUM_OBJECTS: None,
    CommunicationHeaderObjectType.DIMENSION_2D: 2 * 4,
    CommunicationHeaderObjectType.HAS_PAYLOAD: None,
    CommunicationHeaderObjectType.AZIMUTH: 4,
    CommunicationHeaderObjectType.ELEVATION: 4,
    CommunicationHeaderObjectType.AXIS_MIN_MAX_VEC: 6 * 8,
    CommunicationHeaderObjectType.VEC3: None,
    CommunicationHeaderObjectType.SCALE_MATRIX: None,
    CommunicationHeaderObjectType.TRANSLATION_VECTOR: None,
    CommunicationHeaderObjectType.ROTATION_MATRIX: None,
    CommunicationHeaderObjectType.PROJECT_FILE_NAME: None,
    CommunicationHeaderObjectType.TITLE_STRING: None,
    CommunicationHeaderObjectType.POS2D: None,
    CommunicationHeaderObjectType.FIGURE_NUM: None,
    CommunicationHeaderObjectType.PARENT_NAME: None,
    CommunicationHeaderObjectType.PARENT_TYPE: None,
    CommunicationHeaderObjectType.ELEMENT_NAME: 2,
    CommunicationHeaderObjectType.GUI_ELEMENT_TYPE: None,
    CommunicationHeaderObjectType.PROPERTY: None,
    CommunicationHeaderObjectType.ITEM_ID: None,
    CommunicationHeaderObjectType.NUM_NAMES: None,
    CommunicationHeaderObjectType.UNKNOWN: None,
}
