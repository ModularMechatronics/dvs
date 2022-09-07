from enum import Enum
import numpy as np
import sys
import socket
from typing import List, Union

from enums import *
from constants import *
from serialization import *
from structures import *
from internal_types import *

MAX_BYTES_FOR_ONE_MSG = 1380

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


class TransmissionHeader:

    def __init__(self):
        self.properties = {}
        self.settings = {}

    def append(self, setting_name, setting_value):
        self.settings[setting_name] = setting_value

    def append_property(self, property_name, property_value):
        self.properties[property_name] = property_value

    def append_properties(self, properties):
        for key, value in properties.items():
            if key in VALID_PROPERTIES.keys():
                hdr.append_property(VALID_PROPERTIES[key], value)
            else:
                print("Argument {} not a valid property".format(key))

    def num_bytes(self):
        total_size = 0
        for key, val in self.properties.items():
            # + 2 for TransmissionHeaderObjectType, + 1 for num bytes of setting, + 1 for type of property
            total_size += SIZE_OF_PROPERTY[key] + 2 + 1 + 1

        for key, val in self.settings.items():
            # + 2 for TransmissionHeaderObjectType, + 1 for num bytes of setting
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
            bts += TransmissionHeaderObjectType.PROPERTY.value.to_bytes(2, sys.byteorder) + \
                SIZE_OF_PROPERTY[key].to_bytes(1, sys.byteorder) + \
                PROPERTY_SERIALIZATION_FUNCTIONS[key](val)

        return bts


"""
size_t num_sent_bytes = 0;

while (num_sent_bytes < num_bytes)
{
    const size_t num_bytes_to_send =
        std::min(max_bytes_for_one_msg, static_cast<size_t>(num_bytes) - num_sent_bytes);

    udp_client.sendData(&(data_blob[num_sent_bytes]), num_bytes_to_send);
    num_sent_bytes += num_bytes_to_send;

    const int num_received_bytes = udp_client.receiveData(data);

    bool ack_received = checkAck(data);

    if (!ack_received)
    {
        throw std::runtime_error("No ack received!");
    }
    else if (num_received_bytes != 5)
    {
        throw std::runtime_error("Ack received but number of bytes was " + std::to_string(num_received_bytes));
    }
}
"""


def send_with_udp(bts: bytearray):

    PORT_NUM = 9752
    UDP_IP = "127.0.0.1"

    num_bytes = len(bts)
    if num_bytes > MAX_BYTES_FOR_ONE_MSG:
        num_sent_bytes = 0

        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        while num_sent_bytes < num_bytes:
            num_bytes_to_send = min(
                MAX_BYTES_FOR_ONE_MSG, num_bytes - num_sent_bytes)

            sock.sendto(bts[num_sent_bytes:(num_sent_bytes +
                                            num_bytes_to_send)], (UDP_IP, PORT_NUM))
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


def axis(min_vec: Union[Vec2D, Vec3D], max_vec: Union[Vec2D, Vec3D]):

    # TODO: Add if statement if Vec2D is input

    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION, Function.AXES_3D)
    hdr.append(TransmissionHeaderObjectType.AXIS_MIN_MAX_VEC, (min_vec, max_vec))
    send_header(send_with_udp, hdr)


def view(azimuth, elevation):
    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION, Function.VIEW)
    hdr.append(TransmissionHeaderObjectType.AZIMUTH, azimuth)
    hdr.append(TransmissionHeaderObjectType.ELEVATION, elevation)
    send_header(send_with_udp, hdr)


def soft_clear_view():
    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION, Function.SOFT_CLEAR)
    send_header(send_with_udp, hdr)


def clear_view():
    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION, Function.CLEAR)
    send_header(send_with_udp, hdr)


def set_current_element(name: str):

    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION, Function.SET_CURRENT_ELEMENT)
    hdr.append(TransmissionHeaderObjectType.ELEMENT_NAME, name)
    send_header(send_with_udp, hdr)


def hold_on(name: str):

    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION, Function.HOLD_ON)
    send_header(send_with_udp, hdr)


def hold_off(name: str):

    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION, Function.HOLD_OFF)
    send_header(send_with_udp, hdr)

####### Plot functions #######


def plot(x: np.array, y: np.array, **properties):

    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION, Function.PLOT2)
    hdr.append(TransmissionHeaderObjectType.NUM_ELEMENTS, x.size)

    if len(x.shape) == 2:
        hdr.append(TransmissionHeaderObjectType.DATA_TYPE,
                   np_data_type_to_data_type(x[0][0].__class__))
    else:
        hdr.append(TransmissionHeaderObjectType.DATA_TYPE,
                   np_data_type_to_data_type(x[0].__class__))

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, x, y)


def plot3(x: np.array, y: np.array, z: np.array, **properties):

    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION, Function.PLOT3)
    hdr.append(TransmissionHeaderObjectType.NUM_ELEMENTS, x.size)

    if len(x.shape) == 2:
        hdr.append(TransmissionHeaderObjectType.DATA_TYPE,
                   np_data_type_to_data_type(x[0][0].__class__))
    else:
        hdr.append(TransmissionHeaderObjectType.DATA_TYPE,
                   np_data_type_to_data_type(x[0].__class__))

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, x, y, z)


def scatter(x: np.array, y: np.array, **properties):

    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION, Function.SCATTER2)
    hdr.append(TransmissionHeaderObjectType.NUM_ELEMENTS, x.size)

    if len(x.shape) == 2:
        hdr.append(TransmissionHeaderObjectType.DATA_TYPE,
                   np_data_type_to_data_type(x[0][0].__class__))
    else:
        hdr.append(TransmissionHeaderObjectType.DATA_TYPE,
                   np_data_type_to_data_type(x[0].__class__))

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, x, y)


def scatter3(x: np.array, y: np.array, z: np.array, **properties):

    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION, Function.SCATTER3)
    hdr.append(TransmissionHeaderObjectType.NUM_ELEMENTS, x.size)

    if len(x.shape) == 2:
        hdr.append(TransmissionHeaderObjectType.DATA_TYPE,
                   np_data_type_to_data_type(x[0][0].__class__))
    else:
        hdr.append(TransmissionHeaderObjectType.DATA_TYPE,
                   np_data_type_to_data_type(x[0].__class__))

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, x, y, z)


def surf(x: np.array, y: np.array, z: np.array, **properties):
    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION, Function.SURF)
    hdr.append(TransmissionHeaderObjectType.NUM_ELEMENTS, x.size)
    hdr.append(TransmissionHeaderObjectType.DIMENSION_2D, x.shape)

    hdr.append(TransmissionHeaderObjectType.DATA_TYPE,
               np_data_type_to_data_type(x[0][0].__class__))

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, x, y, z)


def imshow(img: np.array, **properties):
    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION, Function.IM_SHOW)

    if len(img.shape) == 2:
        hdr.append(TransmissionHeaderObjectType.NUM_CHANNELS, 1)
        hdr.append(TransmissionHeaderObjectType.DIMENSION_2D, img.shape)
    else:
        hdr.append(TransmissionHeaderObjectType.NUM_CHANNELS, 3)
        hdr.append(TransmissionHeaderObjectType.DIMENSION_2D, img[0].shape)
    hdr.append(TransmissionHeaderObjectType.NUM_ELEMENTS, img.size)

    hdr.append(TransmissionHeaderObjectType.DATA_TYPE,
               np_data_type_to_data_type(img[0][0][0].__class__))

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, img)


def draw_polygon_from_4_points(p0: Point3D, p1: Point3D, p2: Point3D, p3: Point3D, **properties):
    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION,
               Function.POLYGON_FROM_4_POINTS)
    hdr.append(TransmissionHeaderObjectType.NUM_ELEMENTS, 4)

    hdr.append(TransmissionHeaderObjectType.DATA_TYPE, DataType.DOUBLE)

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, p0, p1, p2, p3)


def draw_triangle(triangle: Triangle3D, **properties):

    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION,
               Function.DRAW_TRIANGLES_3D)
    hdr.append(TransmissionHeaderObjectType.NUM_ELEMENTS, 1)
    hdr.append(TransmissionHeaderObjectType.DATA_TYPE, DataType.DOUBLE)

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, triangle)


def draw_triangles(triangles: List[Triangle3D], **properties):
    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION, Function.DRAW_TRIANGLES_3D)
    hdr.append(TransmissionHeaderObjectType.NUM_ELEMENTS, len(triangles))
    # hdr.append(TransmissionHeaderObjectType.DATA_TYPE, ) TODO

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, triangles)


def draw_mesh(points: List[Point3D], indices: List[IndexTriplet], **properties):
    pass


def draw_plane_xy(plane: Plane, p0: PointXY, p1: PointXY, **properties):
    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION,
               Function.PLANE_XY)
    hdr.append(TransmissionHeaderObjectType.NUM_ELEMENTS, 2)
    hdr.append(TransmissionHeaderObjectType.DATA_TYPE, DataType.DOUBLE)

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, p0, p1, plane)


def draw_plane_xz(plane: Plane, p0: PointXZ, p1: PointXZ, **properties):
    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION,
               Function.PLANE_XZ)
    hdr.append(TransmissionHeaderObjectType.NUM_ELEMENTS, 2)
    hdr.append(TransmissionHeaderObjectType.DATA_TYPE, DataType.DOUBLE)

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, p0, p1, plane)


def draw_plane_yz(plane: Plane, p0: PointYZ, p1: PointYZ, **properties):
    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION,
               Function.PLANE_YZ)
    hdr.append(TransmissionHeaderObjectType.NUM_ELEMENTS, 2)
    hdr.append(TransmissionHeaderObjectType.DATA_TYPE, DataType.DOUBLE)

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, p0, p1, plane)


def draw_line(line: Line3D, t0: np.float64, t1: np.float64, **properties):
    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION,
               Function.DRAW_LINE3D)
    hdr.append(TransmissionHeaderObjectType.NUM_ELEMENTS, 0)
    hdr.append(TransmissionHeaderObjectType.DATA_TYPE, DataType.DOUBLE)

    hdr.append_properties(properties)

    p0 = line.eval(t0)
    p1 = line.eval(t1)

    send_header_and_data(send_with_udp, hdr, p0, p1)


def draw_line_2d(line: PLine2D, t0: np.float64, t1: np.float64, **properties):

    hdr = TransmissionHeader()
    hdr.append(TransmissionHeaderObjectType.FUNCTION,
               Function.DRAW_LINE3D)
    hdr.append(TransmissionHeaderObjectType.NUM_ELEMENTS, 0)
    hdr.append(TransmissionHeaderObjectType.DATA_TYPE, DataType.DOUBLE)

    hdr.append_properties(properties)

    p0_2d = line.eval(t0)
    p1_2d = line.eval(t1)
    p0 = Point3D(p0_2d.x, p0_2d.y, 0.0)
    p1 = Point3D(p1_2d.x, p1_2d.y, 0.0)

    send_header_and_data(send_with_udp, hdr, p0, p1)


def draw_line_between_points(p0: Union[Point2D, Point3D], p1: Union[Point2D, Point3D], **properties):

    assert(type(p0) == type(p1))

    hdr = TransmissionHeader()

    if type(p0) == Point3D:
        p0_u = p0
        p1_u = p1
    else:
        p0_u = Point3D(p0.x, p0.y, 0.0)
        p1_u = Point3D(p1.x, p1.y, 0.0)
        
    hdr.append(TransmissionHeaderObjectType.FUNCTION,
               Function.DRAW_LINE3D)
    hdr.append(TransmissionHeaderObjectType.NUM_ELEMENTS, 0)
    hdr.append(TransmissionHeaderObjectType.DATA_TYPE, DataType.DOUBLE)

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, p0_u, p1_u)
