import sys
import numpy as np

from enums import *
from properties import *

NUM_BYTES_FOR_NAME = 23


def serialize_color(col: Color):
    return (
        PropertyType.COLOR.value.to_bytes(1, sys.byteorder)
        + np.uint8(np.round(col.r * 255.0)).tobytes()
        + np.uint8(np.round(col.g * 255.0)).tobytes()
        + np.uint8(np.round(col.b * 255.0)).tobytes()
    )


def serialize_edge_color(edge_color):
    return (
        PropertyType.EDGE_COLOR.value.to_bytes(1, sys.byteorder)
        + np.uint8(np.round(edge_color.r * 255.0)).tobytes()
        + np.uint8(np.round(edge_color.g * 255.0)).tobytes()
        + np.uint8(np.round(edge_color.b * 255.0)).tobytes()
        + np.uint8(edge_color.has_color).tobytes()
    )


def serialize_face_color(face_color):
    return (
        PropertyType.FACE_COLOR.value.to_bytes(1, sys.byteorder)
        + np.uint8(np.round(face_color.r * 255.0)).tobytes()
        + np.uint8(np.round(face_color.g * 255.0)).tobytes()
        + np.uint8(np.round(face_color.b * 255.0)).tobytes()
        + np.uint8(face_color.has_color).tobytes()
    )


def serialize_line_width(line_width):
    return (
        PropertyType.LINE_WIDTH.value.to_bytes(1, sys.byteorder)
        + np.float32(line_width.line_width).tobytes()
    )


def serialize_point_size(point_size):
    return (
        PropertyType.POINT_SIZE.value.to_bytes(1, sys.byteorder)
        + np.float32(point_size.point_size).tobytes()
    )


def serialize_buffer_size(buffer_size):
    return (
        PropertyType.BUFFER_SIZE.value.to_bytes(1, sys.byteorder)
        + np.float32(buffer_size.buffer_size).tobytes()
    )


def serialize_line_style(line_style):
    return (
        PropertyType.LINE_STYLE.value.to_bytes(1, sys.byteorder)
        + np.uint8(line_style.value).tobytes()
    )


def serialize_scatter_style(scatter_style):
    return (
        PropertyType.SCATTER_STYLE.value.to_bytes(1, sys.byteorder)
        + np.uint8(scatter_style.value).tobytes()
    )


def serialize_alpha(alpha: Alpha):
    return (
        PropertyType.ALPHA.value.to_bytes(1, sys.byteorder)
        + np.float32(alpha.alpha).tobytes()
    )


def serialize_z_offset(z_offset: ZOffset):
    return (
        PropertyType.Z_OFFSET.value.to_bytes(1, sys.byteorder)
        + np.float32(z_offset.z_offset).tobytes()
    )


def serialize_transform(t: Transform):
    return (
        PropertyType.TRANSFORM.value.to_bytes(1, sys.byteorder)
        + np.float64(t.rotation_matrix[0, 0]).tobytes()
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
        PropertyType.DISTANCE_FROM.value.to_bytes(1, sys.byteorder)
        + np.float64(dist_from.pt.x).tobytes()
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


def serialize_str(name: str):
    return bytearray(name.encode("utf8"))


def serialize_name(name: str):
    return (
        PropertyType.NAME.value.to_bytes(1, sys.byteorder)
        + np.uint8(len(name.name)).tobytes()
        + bytearray(name.name.encode("utf8"))
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
    PropertyType.EDGE_COLOR: serialize_edge_color,
    PropertyType.FACE_COLOR: serialize_face_color,
    PropertyType.COLOR_MAP: serialize_color_map,
    PropertyType.LINE_WIDTH: serialize_line_width,
    PropertyType.POINT_SIZE: serialize_point_size,
    PropertyType.LINE_STYLE: serialize_line_style,
    PropertyType.SCATTER_STYLE: serialize_scatter_style,
    PropertyType.BUFFER_SIZE: serialize_buffer_size,
    PropertyType.DISTANCE_FROM: serialize_distance_from,
    PropertyType.Z_OFFSET: serialize_z_offset,
    PropertyType.TRANSFORM: serialize_transform,
    PropertyType.ALPHA: serialize_alpha,
    PropertyType.NAME: serialize_name,
}

SIZE_OF_PROPERTY = {
    PropertyType.COLOR: 4,
    PropertyType.EDGE_COLOR: 4,
    PropertyType.FACE_COLOR: 4,
    PropertyType.COLOR_MAP: 2,
    PropertyType.LINE_WIDTH: 2,
    PropertyType.POINT_SIZE: 2,
    PropertyType.BUFFER_SIZE: 3,
    PropertyType.Z_OFFSET: 4,
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
