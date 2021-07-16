from enum import Enum
import numpy as np
import sys
import socket

from enums import *
from constants import *
from serialization import *
from internal_types import *


VALID_PROPERTIES = {"color": PropertyType.COLOR,
                    "edge_color": PropertyType.EDGE_COLOR,
                    "face_color": PropertyType.FACE_COLOR,
                    "color_map": PropertyType.COLOR_MAP,
                    "line_width": PropertyType.LINE_WIDTH,
                    "point_size": PropertyType.POINT_SIZE,
                    "line_style": PropertyType.LINE_STYLE,
                    "alpha": PropertyType.ALPHA,
                    "name": PropertyType.NAME}


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
        raise Exception('Invalid type!')
    return dt_ret


class FunctionHeader:

    def __init__(self):
        self.properties = {}
        self.settings = {}

    def append(self, setting_name, setting_value):
        self.settings[setting_name] = setting_value

    def append_property(self, property_name, property_value):
        self.properties[property_name] = property_value

    def num_bytes(self):
        total_size = 0
        for key, val in self.properties.items():
            # + 2 for FunctionHeaderObjectType, + 1 for num bytes of setting, + 1 for type of property
            total_size += SIZE_OF_PROPERTY[key] + 2 + 1 + 1

        for key, val in self.settings.items():
            # + 2 for FunctionHeaderObjectType, + 1 for num bytes of setting
            total_size += SIZE_OF_FUNCTION_HEADER_OBJECT[key] + 2 + 1

        # + 1 for number of header elements
        return total_size + 1

    def to_bytes(self):
        bts = bytearray()
        bts += (len(self.properties) + len(self.settings)
                ).to_bytes(1, sys.byteorder)

        for key, val in self.settings.items():
            bts += key.value.to_bytes(2, sys.byteorder) + SIZE_OF_FUNCTION_HEADER_OBJECT[key].to_bytes(1, sys.byteorder) \
                + FUNCTION_HEADER_OBJECT_SERIALIZATION_FUNCTION[key](val)

        for key, val in self.properties.items():
            bts += FunctionHeaderObjectType.PROPERTY.value.to_bytes(2, sys.byteorder) + \
                SIZE_OF_PROPERTY[key].to_bytes(1, sys.byteorder) + \
                PROPERTY_SERIALIZATION_FUNCTIONS[key](val)

        return bts


def send_with_udp(bts: bytearray):
    PORT_NUM = 9752
    UDP_IP = "127.0.0.1"

    sock = socket.socket(socket.AF_INET,
                         socket.SOCK_DGRAM)
    sock.sendto(bts, (UDP_IP, PORT_NUM))


class Buffer:
    def __init__(self, num_bytes):
        self.data = bytearray(num_bytes)
        self.idx = 0

    def append(self, new_bytes):
        new_num_bytes = len(new_bytes)

        self.data[self.idx:(self.idx + new_num_bytes)] = new_bytes
        self.idx += new_num_bytes


def num_bytes_from_array(x: np.array):
    return x.itemsize * x.size


def send_header(send_fcn, hdr, *args):
    total_num_bytes = hdr.num_bytes()

    total_num_bytes += 1 + 2 * 8

    buffer_to_send = Buffer(total_num_bytes)

    is_big_endian = sys.byteorder == 'big'

    buffer_to_send.append(is_big_endian.to_bytes(1, sys.byteorder))

    buffer_to_send.append(MAGIC_NUM.to_bytes(8, sys.byteorder))

    buffer_to_send.append(total_num_bytes.to_bytes(8, sys.byteorder))

    buffer_to_send.append(hdr.to_bytes())

    send_fcn(buffer_to_send.data)


def send_header_and_data(send_fcn, hdr, *args):
    total_num_bytes = hdr.num_bytes()

    for arg in args:
        total_num_bytes += num_bytes_from_array(arg)

    total_num_bytes += 1 + 2 * 8

    buffer_to_send = Buffer(total_num_bytes)

    is_big_endian = sys.byteorder == 'big'

    buffer_to_send.append(is_big_endian.to_bytes(1, sys.byteorder))

    buffer_to_send.append(MAGIC_NUM.to_bytes(8, sys.byteorder))

    buffer_to_send.append(total_num_bytes.to_bytes(8, sys.byteorder))

    buffer_to_send.append(hdr.to_bytes())

    for arg in args:
        buffer_to_send.append(arg.tobytes())

    send_fcn(buffer_to_send.data)


"""

hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::AXES_3D);
hdr.append(internal::FunctionHeaderObjectType::AXIS_MIN_MAX_VEC, std::pair<Bound3D, Bound3D>(min_bound, max_bound));
"""


def axis(min_vec, max_vec):

    hdr = FunctionHeader()
    hdr.append(FunctionHeaderObjectType.FUNCTION, Function.AXES_3D)
    hdr.append(FunctionHeaderObjectType.AXIS_MIN_MAX_VEC, (min_vec, max_vec))
    send_header(send_with_udp, hdr)


def view(azimuth, elevation):
    hdr = FunctionHeader()
    hdr.append(FunctionHeaderObjectType.FUNCTION, Function.VIEW)
    hdr.append(FunctionHeaderObjectType.AZIMUTH, azimuth)
    hdr.append(FunctionHeaderObjectType.ELEVATION, elevation)
    send_header(send_with_udp, hdr)


def clear():
    hdr = FunctionHeader()
    hdr.append(FunctionHeaderObjectType.FUNCTION, Function.SOFT_CLEAR)
    send_header(send_with_udp, hdr)


def hard_clear():
    hdr = FunctionHeader()
    hdr.append(FunctionHeaderObjectType.FUNCTION, Function.CLEAR)
    send_header(send_with_udp, hdr)


def set_current_element(name: str):

    hdr = FunctionHeader()
    hdr.append(FunctionHeaderObjectType.FUNCTION, Function.SET_CURRENT_ELEMENT)
    hdr.append(FunctionHeaderObjectType.ELEMENT_NAME, name)
    send_header(send_with_udp, hdr)


####### Plot functions #######

def plot(x: np.array, y: np.array, **properties):

    hdr = FunctionHeader()
    hdr.append(FunctionHeaderObjectType.FUNCTION, Function.PLOT2)
    hdr.append(FunctionHeaderObjectType.NUM_ELEMENTS, x.size)

    if len(x.shape) == 2:
        hdr.append(FunctionHeaderObjectType.DATA_TYPE,
                   np_data_type_to_data_type(x[0][0].__class__))
    else:
        hdr.append(FunctionHeaderObjectType.DATA_TYPE,
                   np_data_type_to_data_type(x[0].__class__))

    for key, value in properties.items():
        if key in VALID_PROPERTIES.keys():
            hdr.append_property(VALID_PROPERTIES[key], value)
        else:
            print("Argument {} not a valid property".format(key))

    send_header_and_data(send_with_udp, hdr, x, y)


def plot3(x: np.array, y: np.array, z: np.array, **properties):

    hdr = FunctionHeader()
    hdr.append(FunctionHeaderObjectType.FUNCTION, Function.PLOT3)
    hdr.append(FunctionHeaderObjectType.NUM_ELEMENTS, x.size)

    if len(x.shape) == 2:
        hdr.append(FunctionHeaderObjectType.DATA_TYPE,
                   np_data_type_to_data_type(x[0][0].__class__))
    else:
        hdr.append(FunctionHeaderObjectType.DATA_TYPE,
                   np_data_type_to_data_type(x[0].__class__))

    for key, value in properties.items():
        if key in VALID_PROPERTIES.keys():
            hdr.append_property(VALID_PROPERTIES[key], value)
        else:
            print("Argument {} not a valid property".format(key))

    send_header_and_data(send_with_udp, hdr, x, y, z)


def surf(x: np.array, y: np.array, z: np.array, **properties):
    hdr = FunctionHeader()
    hdr.append(FunctionHeaderObjectType.FUNCTION, Function.SURF)
    hdr.append(FunctionHeaderObjectType.NUM_ELEMENTS, x.size)
    hdr.append(FunctionHeaderObjectType.DIMENSION_2D, x.shape)

    hdr.append(FunctionHeaderObjectType.DATA_TYPE,
               np_data_type_to_data_type(x[0][0].__class__))

    for key, value in properties.items():
        if key in VALID_PROPERTIES.keys():
            hdr.append_property(VALID_PROPERTIES[key], value)
        else:
            print("Argument {} not a valid property".format(key))

    send_header_and_data(send_with_udp, hdr, x, y, z)
