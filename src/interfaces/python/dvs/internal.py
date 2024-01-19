import numpy as np
from typing import List, Union
import properties
from enum import Enum
import sys
import socket
import struct
from enums import *
from constants import *
from serialization import *
from structures import *


def get_dtype_from_element(elem):
    if isinstance(elem, float):
        return np.float64
    elif isinstance(elem, int):
        return np.int64
    elif elem.__class__ in [
        np.int8,
        np.int16,
        np.int32,
        np.int64,
        np.uint8,
        np.uint16,
        np.uint32,
        np.uint64,
        np.float32,
        np.float64,
    ]:
        return elem.__class__
    else:
        raise Exception(f"Invalid type: {elem.__class__}")


def clean_input_vec(x: Union[np.array, list, tuple]):
    if isinstance(x, list) or isinstance(x, tuple):
        return np.array(x, dtype=get_dtype_from_element(x[0]))
    else:
        if len(x.shape) > 1:
            if x.shape[0] == 1 or x.shape[1] == 1:
                return x.flatten()
            else:
                Exception(f"Invalid shape for input vector: {x.shape}")
        else:
            return x


def ensure_vec3d(
    vec: Union[properties.Vec2D, properties.Vec3D, list, np.ndarray, tuple],
    z_val: float,
):
    if isinstance(vec, list) or isinstance(vec, tuple):
        if len(vec) == 3:
            return properties.Vec3D(vec[0], vec[1], vec[2])
        elif len(vec) == 2:
            return properties.Vec3D(vec[0], vec[1], z_val)
        else:
            raise Exception(f"Invalid shape for input to axis: {vec}")

    elif isinstance(vec, np.ndarray):
        if vec.shape == (3,):
            return properties.Vec3D(vec[0], vec[1], vec[2])
        elif vec.shape == (3, 1):
            return properties.Vec3D(vec[0][0], vec[1][0], vec[2][0])
        elif vec.shape == (1, 3):
            return properties.Vec3D(vec[0][0], vec[0][1], vec[0][2])
        elif vec.shape == (2,):
            return properties.Vec3D(vec[0], vec[1], z_val)
        elif vec.shape == (2, 1):
            return properties.Vec3D(vec[0][0], vec[1][0], z_val)
        elif vec.shape == (1, 2):
            return properties.Vec3D(vec[0][0], vec[0][1], z_val)
        else:
            raise Exception(f"Invalid shape for input to axis: {vec}")
    else:
        return vec


MAX_BYTES_FOR_ONE_MSG = 1380

VALID_PROPERTIES = {
    "color": PropertyType.COLOR,
    "edge_color": PropertyType.EDGE_COLOR,
    "face_color": PropertyType.FACE_COLOR,
    "color_map": PropertyType.COLOR_MAP,
    "line_width": PropertyType.LINE_WIDTH,
    "point_size": PropertyType.POINT_SIZE,
    "line_style": PropertyType.LINE_STYLE,
    "alpha": PropertyType.ALPHA,
    "name": PropertyType.NAME,
    "buffer_size": PropertyType.BUFFER_SIZE,
    "z_offset": PropertyType.Z_OFFSET,
    "distance_from": PropertyType.DISTANCE_FROM,
    "scatter_style": PropertyType.SCATTER_STYLE,
    "transform": PropertyType.TRANSFORM,
}


def np_data_type_to_size(dt: type):
    dt_size = 0

    if dt == np.float32:
        dt_size = 4
    elif dt == np.float64:
        dt_size = 8
    elif dt == np.int8:
        dt_size = 1
    elif dt == np.int16:
        dt_size = 2
    elif dt == np.int32:
        dt_size = 4
    elif dt == np.int64:
        dt_size = 8
    elif dt == np.uint8:
        dt_size = 1
    elif dt == np.uint16:
        dt_size = 2
    elif dt == np.uint32:
        dt_size = 4
    elif dt == np.uint64:
        dt_size = 8
    else:
        raise Exception("Invalid type!")

    return dt_size


def np_data_type_to_data_type(dt: type):
    dt_ret = DataType.UNKNOWN

    if dt == np.float32:
        dt_ret = DataType.FLOAT
    elif dt == np.float64:
        dt_ret = DataType.DOUBLE
    elif dt == np.int8:
        dt_ret = DataType.INT8
    elif dt == np.int16:
        dt_ret = DataType.INT16
    elif dt == np.int32:
        dt_ret = DataType.INT32
    elif dt == np.int64:
        dt_ret = DataType.INT64
    elif dt == np.uint8:
        dt_ret = DataType.UINT8
    elif dt == np.uint16:
        dt_ret = DataType.UINT16
    elif dt == np.uint32:
        dt_ret = DataType.UINT32
    elif dt == np.uint64:
        dt_ret = DataType.UINT64
    else:
        raise Exception("Invalid type!")
    return dt_ret


COMMUNICATION_HEADER_OBJECT_LOOKUP_TABLE_SIZE = (
    CommunicationHeaderObjectType.UNKNOWN.value + 1
)
PROPERTY_LOOKUP_TABLE_SIZE = PropertyType.UNKNOWN.value + 1


class CommunicationHeaderObjectLookupTable:
    def __init__(self):
        self.data = COMMUNICATION_HEADER_OBJECT_LOOKUP_TABLE_SIZE * [255]

    def append_object_index(self, type: CommunicationHeaderObjectType, idx):
        self.data[type.value] = idx

    def clear(self):
        self.data = COMMUNICATION_HEADER_OBJECT_LOOKUP_TABLE_SIZE * [255]


class PropertyLookupTable:
    def __init__(self):
        self.data = PROPERTY_LOOKUP_TABLE_SIZE * [255]

    def append_property_index(self, type: PropertyType, idx):
        self.data[type.value] = idx

    def clear(self):
        self.data = PROPERTY_LOOKUP_TABLE_SIZE * [255]


def edge_color_t_to_edge_color(edge_color_t: EdgeColorT):
    ec = EdgeColor(0.0, 0.0, 0.0)

    if edge_color_t == EdgeColorT.RED:
        ec.r = 1.0
        ec.g = 0.0
        ec.b = 0.0
    elif edge_color_t == EdgeColorT.GREEN:
        ec.r = 0.0
        ec.g = 1.0
        ec.b = 0.0
    elif edge_color_t == EdgeColorT.BLUE:
        ec.r = 0.0
        ec.g = 0.0
        ec.b = 1.0
    elif edge_color_t == EdgeColorT.CYAN:
        ec.r = 0.0
        ec.g = 1.0
        ec.b = 1.0
    elif edge_color_t == EdgeColorT.MAGENTA:
        ec.r = 1.0
        ec.g = 0.0
        ec.b = 1.0
    elif edge_color_t == EdgeColorT.YELLOW:
        ec.r = 1.0
        ec.g = 1.0
        ec.b = 0.0
    elif edge_color_t == EdgeColorT.BLACK:
        ec.r = 0.0
        ec.g = 0.0
        ec.b = 0.0
    elif edge_color_t == EdgeColorT.WHITE:
        ec.r = 1.0
        ec.g = 1.0
        ec.b = 1.0
    elif edge_color_t == EdgeColorT.GRAY:
        ec.r = 0.5
        ec.g = 0.5
        ec.b = 0.5
    elif edge_color_t == EdgeColorT.NONE:
        ec.has_color = False

    return ec


def face_color_t_to_face_color(face_color_t: FaceColorT):
    fc = FaceColor(0.0, 0.0, 0.0)

    if face_color_t == FaceColorT.RED:
        fc.r = 1.0
        fc.g = 0.0
        fc.b = 0.0
    elif face_color_t == FaceColorT.GREEN:
        fc.r = 0.0
        fc.g = 1.0
        fc.b = 0.0
    elif face_color_t == FaceColorT.BLUE:
        fc.r = 0.0
        fc.g = 0.0
        fc.b = 1.0
    elif face_color_t == FaceColorT.CYAN:
        fc.r = 0.0
        fc.g = 1.0
        fc.b = 1.0
    elif face_color_t == FaceColorT.MAGENTA:
        fc.r = 1.0
        fc.g = 0.0
        fc.b = 1.0
    elif face_color_t == FaceColorT.YELLOW:
        fc.r = 1.0
        fc.g = 1.0
        fc.b = 0.0
    elif face_color_t == FaceColorT.BLACK:
        fc.r = 0.0
        fc.g = 0.0
        fc.b = 0.0
    elif face_color_t == FaceColorT.WHITE:
        fc.r = 1.0
        fc.g = 1.0
        fc.b = 1.0
    elif face_color_t == FaceColorT.GRAY:
        fc.r = 0.5
        fc.g = 0.5
        fc.b = 0.5
    elif face_color_t == FaceColorT.NONE:
        fc.has_color = False

    return fc


def color_t_to_color(color_t: ColorT):
    c = Color(0.0, 0.0, 0.0)

    if color_t == ColorT.RED:
        c.r = 1.0
        c.g = 0.0
        c.b = 0.0
    elif color_t == ColorT.GREEN:
        c.r = 0.0
        c.g = 1.0
        c.b = 0.0
    elif color_t == ColorT.BLUE:
        c.r = 0.0
        c.g = 0.0
        c.b = 1.0
    elif color_t == ColorT.CYAN:
        c.r = 0.0
        c.g = 1.0
        c.b = 1.0
    elif color_t == ColorT.MAGENTA:
        c.r = 1.0
        c.g = 0.0
        c.b = 1.0
    elif color_t == ColorT.YELLOW:
        c.r = 1.0
        c.g = 1.0
        c.b = 0.0
    elif color_t == ColorT.BLACK:
        c.r = 0.0
        c.g = 0.0
        c.b = 0.0
    elif color_t == ColorT.WHITE:
        c.r = 1.0
        c.g = 1.0
        c.b = 1.0
    elif color_t == ColorT.GRAY:
        c.r = 0.5
        c.g = 0.5
        c.b = 0.5

    return c


class CommunicationHeader:
    def __init__(self, function):
        self.objects = []
        self.props = []
        self.flags = (PropertyFlag.UNKNOWN.value + 1) * [0]

        self.objects_lut = CommunicationHeaderObjectLookupTable()
        self.properties_lut = PropertyLookupTable()

        self.obj_idx = 0
        self.prop_idx = 0
        self.function = function

    def append(self, object_type, object_value):
        self.objects_lut.append_object_index(object_type, self.obj_idx)
        self.obj_idx += 1
        self.objects.append((object_type, object_value))

    def append_property(self, property_type, property_value):
        if property_type in PROPERTY_TYPE_TO_CLASS:
            property_value_to_add = PROPERTY_TYPE_TO_CLASS[property_type](
                property_value
            )
        elif isinstance(property_value, EdgeColorT):
            property_value_to_add = edge_color_t_to_edge_color(property_value)
        elif isinstance(property_value, FaceColorT):
            property_value_to_add = face_color_t_to_face_color(property_value)
        elif isinstance(property_value, ColorT):
            property_value_to_add = color_t_to_color(property_value)
        else:
            property_value_to_add = property_value

        self.props.append((property_type, property_value_to_add))

        self.properties_lut.append_property_index(property_type, self.prop_idx)
        self.prop_idx += 1

    def append_properties(self, props):
        for key, value in props.items():
            if key in VALID_PROPERTIES.keys():
                self.append_property(VALID_PROPERTIES[key], value)
            else:
                print("Argument {} not a valid property".format(key))

    def to_bytes(self):
        bts = bytearray()

        bts += np.uint8(len(self.objects)).tobytes()  # Objects
        bts += np.uint8(len(self.props)).tobytes()  # Properties

        bts += self.function.value.to_bytes(1, sys.byteorder)  # Function

        bts += bytearray(
            [np.uint8(q) for q in self.objects_lut.data]
        )  # Objects lookup table

        bts += bytearray(
            [np.uint8(q) for q in self.properties_lut.data]
        )  # Properties lookup table

        for key, val in self.objects:
            bts += key.value.to_bytes(2, sys.byteorder)

            if key == CommunicationHeaderObjectType.ELEMENT_NAME:
                bts += (SIZE_OF_FUNCTION_HEADER_OBJECT[key] + len(val)).to_bytes(
                    1, sys.byteorder
                )
            else:
                bts += SIZE_OF_FUNCTION_HEADER_OBJECT[key].to_bytes(1, sys.byteorder)

            bts += FUNCTION_HEADER_OBJECT_SERIALIZATION_FUNCTION[key](val)

        for key, val in self.props:
            # Type
            bts += CommunicationHeaderObjectType.PROPERTY.value.to_bytes(
                2, sys.byteorder
            )
            # Number of bytes
            bts += SIZE_OF_PROPERTY[key].to_bytes(1, sys.byteorder)

            # Data payload
            bts += key.value.to_bytes(1, sys.byteorder)
            bts += PROPERTY_SERIALIZATION_FUNCTIONS[key](val)

        bts += bytearray([np.uint8(q) for q in self.flags])  # Flags

        return bts

socket_fd = None

def initialize_tcp_socket():
    global socket_fd

    TCP_IP = "127.0.0.1"
    PORT_NUM = 9755

    socket_fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    socket_fd.connect((TCP_IP, PORT_NUM))

def send_with_tcp(bts: bytearray):
    global socket_fd

    if socket_fd is None:
        initialize_tcp_socket()

    num_bytes_to_send = np.uint64(len(bts))

    socket_fd.send(struct.pack("<Q", num_bytes_to_send))
    socket_fd.send(bts)


def send_with_udp(bts: bytearray):
    PORT_NUM = 9752
    UDP_IP = "127.0.0.1"

    num_bytes = len(bts)
    if num_bytes > MAX_BYTES_FOR_ONE_MSG:
        num_sent_bytes = 0

        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        while num_sent_bytes < num_bytes:
            num_bytes_to_send = min(MAX_BYTES_FOR_ONE_MSG, num_bytes - num_sent_bytes)

            sock.sendto(
                bts[num_sent_bytes : (num_sent_bytes + num_bytes_to_send)],
                (UDP_IP, PORT_NUM),
            )
            if sock.recvfrom(4)[0].decode("utf-8") != "ack#":
                raise Exception("Didn't receive ack!")
            num_sent_bytes += num_bytes_to_send
    else:
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.sendto(bts, (UDP_IP, PORT_NUM))
        if sock.recvfrom(4)[0].decode("utf-8") != "ack#":
            raise Exception("Didn't receive ack!")


class Buffer:
    def __init__(self, num_bytes):
        self.data = bytearray(num_bytes)
        self.idx = 0

    def append(self, new_bytes):
        new_num_bytes = len(new_bytes)

        self.data[self.idx : (self.idx + new_num_bytes)] = new_bytes
        self.idx += new_num_bytes


def num_bytes_from_array(x: np.array):
    return x.itemsize * x.size


def send_header(send_fcn, hdr, *args):
    hdr_bytes = hdr.to_bytes()
    total_num_bytes = len(hdr_bytes)

    total_num_bytes += 1 + 2 * 8

    buffer_to_send = Buffer(total_num_bytes)

    is_big_endian = sys.byteorder == "big"

    buffer_to_send.append(is_big_endian.to_bytes(1, sys.byteorder))

    buffer_to_send.append(MAGIC_NUM.to_bytes(8, sys.byteorder))

    buffer_to_send.append(total_num_bytes.to_bytes(8, sys.byteorder))

    buffer_to_send.append(hdr_bytes)

    send_fcn(buffer_to_send.data)


def send_header_and_vector_collection(
    send_fcn, hdr, num_bytes_to_send, vector_lengths, *args
):
    hdr_bytes = hdr.to_bytes()
    total_num_bytes = len(hdr_bytes)

    total_num_bytes += num_bytes_to_send

    total_num_bytes += 1 + 2 * 8

    total_num_bytes += len(vector_lengths) * np_data_type_to_size(np.uint16)

    buffer_to_send = Buffer(total_num_bytes)

    is_big_endian = sys.byteorder == "big"

    buffer_to_send.append(is_big_endian.to_bytes(1, sys.byteorder))

    buffer_to_send.append(MAGIC_NUM.to_bytes(8, sys.byteorder))

    buffer_to_send.append(total_num_bytes.to_bytes(8, sys.byteorder))

    buffer_to_send.append(hdr_bytes)

    buffer_to_send.append(np.array([np.uint16(v) for v in vector_lengths]).tobytes())

    for arg in args:
        buffer_to_send.append(arg.tobytes())

    send_fcn(buffer_to_send.data)


def send_header_and_data(send_fcn, hdr, *args):
    hdr_bytes = hdr.to_bytes()
    total_num_bytes = len(hdr_bytes)

    for arg in args:
        total_num_bytes += num_bytes_from_array(arg)

    total_num_bytes += 1 + 2 * 8

    buffer_to_send = Buffer(total_num_bytes)

    is_big_endian = sys.byteorder == "big"

    buffer_to_send.append(is_big_endian.to_bytes(1, sys.byteorder))

    buffer_to_send.append(MAGIC_NUM.to_bytes(8, sys.byteorder))

    buffer_to_send.append(total_num_bytes.to_bytes(8, sys.byteorder))

    buffer_to_send.append(hdr_bytes)

    for arg in args:
        buffer_to_send.append(arg.tobytes())

    send_fcn(buffer_to_send.data)


def clean_surf_input(x: Union[np.array, list, tuple]):
    if isinstance(x, list) or isinstance(x, tuple):
        # TODO: No assertion is made to ensure that it's a list of lists
        # or a tuple of tuples
        return np.array(x, dtype=get_dtype_from_element(x[0][0]))
    else:
        return x.astype(get_dtype_from_element(x[0][0]))
