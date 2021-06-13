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
            # + 2 for FunctionHeaderObjectType, + 1 for PropertyType
            total_size += SIZE_OF_PROPERTY[key] + 2 + 1 + 1

        for key, val in self.settings.items():
            # + 2 for FunctionHeaderObjectType
            total_size += SIZE_OF_FUNCTION_HEADER_OBJECT[key] + 2 + 1
        # + 1 for number of header elements
        return total_size + 1

    def to_bytes(self):
        bts = bytearray()
        bts += (len(self.properties) + len(self.settings)).to_bytes(1, sys.byteorder)
        for key, val in self.properties.items():
            bts += FunctionHeaderObjectType.PROPERTY.value.to_bytes(2, sys.byteorder) + \
                    key.value.to_bytes(1, sys.byteorder) + PROPERTY_SERIALIZATION_FUNCTIONS[key](val)

        for key, val in self.settings.items():
            bts += key.value.to_bytes(2, sys.byteorder) + SIZE_OF_FUNCTION_HEADER_OBJECT[key].to_bytes(1, sys.byteorder) \
                 + FUNCTION_HEADER_OBJECT_SERIALIZATION_FUNCTION[key](val)

        return bts


def send_with_udp(bytes: bytearray):
    PORT_NUM = 9752
    UDP_IP = "127.0.0.1"

    sock = socket.socket(socket.AF_INET,
                         socket.SOCK_DGRAM)
    sock.sendto(bytes, (UDP_IP, PORT_NUM))


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


def send_header_and_data(send_fcn, hdr, *args):
    total_num_bytes = hdr.num_bytes()

    for arg in args:
        total_num_bytes += num_bytes_from_array(arg)

    total_num_bytes += 1 + 2 * 8
    print("Total num bytes: {}".format(total_num_bytes))
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
    const uint64_t num_bytes_hdr = hdr.numBytes();
    const uint64_t num_bytes_first = first_element.numBytes();

    const uint64_t num_bytes_from_object = countNumBytes(other_elements...) + num_bytes_hdr + num_bytes_first;

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_from_object + 1 + 2 * sizeof(uint64_t);

    uint8_t* const data_blob = new uint8_t[num_bytes];

    uint64_t idx = 0;
    data_blob[idx] = isBigEndian();
    idx += 1;

    std::memcpy(&(data_blob[idx]), &magic_num, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    std::memcpy(&(data_blob[sizeof(uint64_t) + 1]), &num_bytes, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    hdr.fillBufferWithData(&(data_blob[idx]));
    idx += num_bytes_hdr;

    first_element.fillBufferWithData(&(data_blob[idx]));
    idx += num_bytes_first;

    fillBuffer(&(data_blob[idx]), other_elements...);

    send_function(data_blob, num_bytes);
    """


def plot(x: np.array, y: np.array, **properties):

    hdr = FunctionHeader()
    hdr.append(FunctionHeaderObjectType.FUNCTION, Function.PLOT2)
    hdr.append(FunctionHeaderObjectType.NUM_ELEMENTS, x.size)

    if x.shape == 2:
        hdr.append(FunctionHeaderObjectType.DATA_TYPE, np_data_type_to_data_type(x[0][0].__class__))
    else:
        hdr.append(FunctionHeaderObjectType.DATA_TYPE, np_data_type_to_data_type(x[0].__class__))

    for key, value in properties.items():
        if key in VALID_PROPERTIES.keys():
            hdr.append_property(VALID_PROPERTIES[key], value)
        else:
            print("Argument {} not a valid property".format(key))

    send_header_and_data(send_with_udp, hdr, x, y)

    """
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::PLOT2);
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
    """