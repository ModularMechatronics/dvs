import sys
import numpy as np

from enums import *
from properties import *

NUM_BYTES_FOR_NAME = 23


def serialize_color(col: Color):
    return (
        np.uint8(np.round(col.r * 255.0)).tobytes()
        + np.uint8(np.round(col.g * 255.0)).tobytes()
        + np.uint8(np.round(col.b * 255.0)).tobytes()
    )


def serialize_edge_color(edge_color):
    return (
        np.uint8(np.round(edge_color.r * 255.0)).tobytes()
        + np.uint8(np.round(edge_color.g * 255.0)).tobytes()
        + np.uint8(np.round(edge_color.b * 255.0)).tobytes()
        + np.uint8(edge_color.has_color).tobytes()
    )


def serialize_face_color(face_color):
    return (
        np.uint8(np.round(face_color.r * 255.0)).tobytes()
        + np.uint8(np.round(face_color.g * 255.0)).tobytes()
        + np.uint8(np.round(face_color.b * 255.0)).tobytes()
        + np.uint8(face_color.has_color).tobytes()
    )


def serialize_transform(t: Transform):
    return (
        np.float64(t.rotation_matrix[0, 0]).tobytes()
        + np.float64(t.rotation_matrix[0, 1]).tobytes()
        + np.float64(t.rotation_matrix[0, 2]).tobytes()
        + np.float64(t.rotation_matrix[1, 0]).tobytes()
        + np.float64(t.rotation_matrix[1, 1]).tobytes()
        + np.float64(t.rotation_matrix[1, 2]).tobytes()
        + np.float64(t.rotation_matrix[2, 0]).tobytes()
        + np.float64(t.rotation_matrix[2, 1]).tobytes()
        + np.float64(t.rotation_matrix[2, 2]).tobytes()
        + np.float64(t.scale_matrix[0, 0]).tobytes()
        + np.float64(t.scale_matrix[0, 1]).tobytes()
        + np.float64(t.scale_matrix[0, 2]).tobytes()
        + np.float64(t.scale_matrix[1, 0]).tobytes()
        + np.float64(t.scale_matrix[1, 1]).tobytes()
        + np.float64(t.scale_matrix[1, 2]).tobytes()
        + np.float64(t.scale_matrix[2, 0]).tobytes()
        + np.float64(t.scale_matrix[2, 1]).tobytes()
        + np.float64(t.scale_matrix[2, 2]).tobytes()
        + np.float64(t.translation_vector.flatten()[0]).tobytes()
        + np.float64(t.translation_vector.flatten()[1]).tobytes()
        + np.float64(t.translation_vector.flatten()[2]).tobytes()
    )


def serialize_distance_from(dist_from: DistanceFrom):
    return (
        np.float64(dist_from.pt.x).tobytes()
        + np.float64(dist_from.pt.y).tobytes()
        + np.float64(dist_from.pt.z).tobytes()
        + np.float64(dist_from.min_distance).tobytes()
        + np.float64(dist_from.max_distance).tobytes()
        + np.uint8(dist_from.distance_from_type.value).tobytes()
    )


def serialize_one_byte_num(num):
    return np.uint8(num).tobytes()


def serialize_two_byte_num(num):
    return np.uint16(num).tobytes()


def serialize_float32(num):
    return np.float32(num).tobytes()


def serialize_float64(num):
    return np.float64(num).tobytes()


def serialize_name(name: str):
    return np.uint8(len(name.name)).tobytes() + bytearray(name.name.encode("utf8"))


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
    PropertyType.EDGE_COLOR: serialize_edge_color,
    PropertyType.FACE_COLOR: serialize_face_color,
    PropertyType.COLOR_MAP: lambda x: serialize_one_byte_num(x.value),
    PropertyType.LINE_WIDTH: lambda x: serialize_one_byte_num(x.line_width),
    PropertyType.POINT_SIZE: lambda x: serialize_one_byte_num(x.point_size),
    PropertyType.LINE_STYLE: lambda x: serialize_one_byte_num(x.value),
    PropertyType.SCATTER_STYLE: lambda x: serialize_one_byte_num(x.value),
    PropertyType.BUFFER_SIZE: lambda x: serialize_two_byte_num(x.buffer_size),
    PropertyType.DISTANCE_FROM: serialize_distance_from,
    PropertyType.Z_OFFSET: lambda x: serialize_float32(x.z_offset),
    PropertyType.TRANSFORM: serialize_transform,
    PropertyType.ALPHA: lambda x: serialize_float32(x.alpha),
    PropertyType.NAME: serialize_name,
}

SIZE_OF_PROPERTY = {
    PropertyType.COLOR: 4,
    PropertyType.EDGE_COLOR: 5,
    PropertyType.FACE_COLOR: 5,
    PropertyType.COLOR_MAP: 2,
    PropertyType.LINE_WIDTH: 2,
    PropertyType.POINT_SIZE: 2,
    PropertyType.BUFFER_SIZE: 3,
    PropertyType.Z_OFFSET: 5,
    PropertyType.DISTANCE_FROM: 1 + 3 * 8 + 8 + 8 + 1,
    PropertyType.LINE_STYLE: 2,
    PropertyType.SCATTER_STYLE: 2,
    PropertyType.TRANSFORM: 1 + 8 + 3 * 3 + 3 * 3 + 3,
    PropertyType.ALPHA: 5,
    PropertyType.NAME: 10,
}


FUNCTION_HEADER_OBJECT_SERIALIZATION_FUNCTION = {
    CommunicationHeaderObjectType.FUNCTION: None,
    CommunicationHeaderObjectType.NUM_BUFFERS_REQUIRED: None,
    CommunicationHeaderObjectType.NUM_BYTES: None,
    CommunicationHeaderObjectType.DATA_STRUCTURE: None,
    CommunicationHeaderObjectType.BYTES_PER_ELEMENT: lambda x: np.uint8(x).tobytes(),
    CommunicationHeaderObjectType.DATA_TYPE: lambda x: np.uint8(x.value).tobytes(),
    CommunicationHeaderObjectType.NUM_CHANNELS: lambda x: np.uint8(x).tobytes(),
    CommunicationHeaderObjectType.NUM_ELEMENTS: lambda x: np.uint32(x).tobytes(),
    CommunicationHeaderObjectType.HAS_COLOR: None,
    CommunicationHeaderObjectType.HAS_POINT_SIZES: None,
    CommunicationHeaderObjectType.NUM_VERTICES: lambda x: np.uint32(x).tobytes(),
    CommunicationHeaderObjectType.NUM_INDICES: lambda x: np.uint32(x).tobytes(),
    CommunicationHeaderObjectType.NUM_OBJECTS: lambda x: np.uint32(x).tobytes(),
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
    CommunicationHeaderObjectType.ITEM_ID: lambda x: np.uint16(x.value).tobytes(),
    CommunicationHeaderObjectType.NUM_NAMES: None,
    CommunicationHeaderObjectType.UNKNOWN: None,
}


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
    CommunicationHeaderObjectType.NUM_VERTICES: 4,
    CommunicationHeaderObjectType.NUM_INDICES: 4,
    CommunicationHeaderObjectType.NUM_OBJECTS: 4,
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
    CommunicationHeaderObjectType.ITEM_ID: 2,
    CommunicationHeaderObjectType.NUM_NAMES: None,
    CommunicationHeaderObjectType.UNKNOWN: None,
}
