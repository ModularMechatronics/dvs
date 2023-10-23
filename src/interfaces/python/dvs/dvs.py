import numpy as np
from typing import List, Union, Tuple

from enums import *
from constants import *
from serialization import *
from structures import *
import internal


def plot(
    x: Union[np.array, List, Tuple],
    y: Union[np.array, List, Tuple],
    **props_kw,
):
    x = internal.clean_input_vec(x)
    y = internal.clean_input_vec(y)

    if x.shape != y.shape:
        raise Exception(
            "x and y must have same shape! Shapes are x: {x.shape}, y: {y.shape}"
        )

    hdr = internal.CommunicationHeader(Function.PLOT2)
    hdr.append(CommunicationHeaderObjectType.NUM_ELEMENTS, x.size)

    hdr.append(
        CommunicationHeaderObjectType.DATA_TYPE,
        internal.np_data_type_to_data_type(x[0].__class__),
    )

    hdr.append_properties(props_kw)

    internal.send_header_and_data(internal.send_with_tcp, hdr, x, y)


def plot3(
    x: Union[np.array, List, Tuple],
    y: Union[np.array, List, Tuple],
    z: Union[np.array, List, Tuple],
    **props_kw,
):
    x = internal.clean_input_vec(x)
    y = internal.clean_input_vec(y)
    z = internal.clean_input_vec(z)

    if x.shape != y.shape or x.shape != z.shape:
        raise Exception(
            "x and y must have same shape! Shapes are x: {x.shape}, y: {y.shape}, z: {z.shape}"
        )

    hdr = internal.CommunicationHeader(Function.PLOT3)
    hdr.append(CommunicationHeaderObjectType.NUM_ELEMENTS, x.size)

    hdr.append(
        CommunicationHeaderObjectType.DATA_TYPE,
        internal.np_data_type_to_data_type(x[0].__class__),
    )

    hdr.append_properties(props_kw)

    internal.send_header_and_data(internal.send_with_tcp, hdr, x, y, z)


def scatter(
    x: Union[np.array, List, Tuple], y: Union[np.array, List, Tuple], **props_kw
):
    x = internal.clean_input_vec(x)
    y = internal.clean_input_vec(y)

    if x.shape != y.shape:
        raise Exception(
            "x and y must have same shape! Shapes are x: {x.shape}, y: {y.shape}"
        )

    hdr = internal.CommunicationHeader(Function.SCATTER2)
    hdr.append(CommunicationHeaderObjectType.NUM_ELEMENTS, x.size)

    hdr.append(
        CommunicationHeaderObjectType.DATA_TYPE,
        internal.np_data_type_to_data_type(x[0].__class__),
    )

    hdr.append_properties(props_kw)

    internal.send_header_and_data(internal.send_with_tcp, hdr, x, y)


def scatter3(
    x: Union[np.array, List, Tuple],
    y: Union[np.array, List, Tuple],
    z: Union[np.array, List, Tuple],
    **props_kw,
):
    x = internal.clean_input_vec(x)
    y = internal.clean_input_vec(y)
    z = internal.clean_input_vec(z)

    if x.shape != y.shape or x.shape != z.shape:
        raise Exception(
            "x and y must have same shape! Shapes are x: {x.shape}, y: {y.shape}, z: {z.shape}"
        )

    hdr = internal.CommunicationHeader(Function.SCATTER3)
    hdr.append(CommunicationHeaderObjectType.NUM_ELEMENTS, x.size)

    if len(x.shape) == 2:
        hdr.append(
            CommunicationHeaderObjectType.DATA_TYPE,
            internal.np_data_type_to_data_type(x[0][0].__class__),
        )
    else:
        hdr.append(
            CommunicationHeaderObjectType.DATA_TYPE,
            internal.np_data_type_to_data_type(x[0].__class__),
        )

    hdr.append_properties(props_kw)

    internal.send_header_and_data(internal.send_with_tcp, hdr, x, y, z)


def surf(
    x: Union[np.array, List[List], Tuple[Tuple]],
    y: Union[np.array, List[List], Tuple[Tuple]],
    z: Union[np.array, List[List], Tuple[Tuple]],
    **props_kw,
):
    x = internal.clean_surf_input(x)
    y = internal.clean_surf_input(y)
    z = internal.clean_surf_input(z)

    if x.shape != y.shape or x.shape != z.shape:
        raise Exception("x, y, and z must have same shape!")

    if x.shape[0] == 1 or x.shape[1] == 1:
        Exception(
            f"Invalid shape for input vector: {x.shape}! Both dimensions must be greater than 1."
        )

    hdr = internal.CommunicationHeader(Function.SURF)
    hdr.append(CommunicationHeaderObjectType.NUM_ELEMENTS, x.size)
    hdr.append(CommunicationHeaderObjectType.DIMENSION_2D, x.shape)

    hdr.append(
        CommunicationHeaderObjectType.DATA_TYPE,
        internal.np_data_type_to_data_type(x[0][0].__class__),
    )

    hdr.append_properties(props_kw)

    internal.send_header_and_data(internal.send_with_tcp, hdr, x, y, z)


def imshow(img: np.array, **props_kw):
    hdr = internal.CommunicationHeader(Function.IM_SHOW)

    if len(img.shape) == 2:
        hdr.append(CommunicationHeaderObjectType.NUM_CHANNELS, 1)
        hdr.append(CommunicationHeaderObjectType.DIMENSION_2D, img.shape)
    else:
        hdr.append(CommunicationHeaderObjectType.NUM_CHANNELS, 3)
        hdr.append(CommunicationHeaderObjectType.DIMENSION_2D, img[0].shape)
    hdr.append(CommunicationHeaderObjectType.NUM_ELEMENTS, img.size)

    hdr.append(
        CommunicationHeaderObjectType.DATA_TYPE,
        internal.np_data_type_to_data_type(img[0][0][0].__class__),
    )

    hdr.append_properties(props_kw)

    internal.send_header_and_data(internal.send_with_tcp, hdr, img)


def draw_mesh(points: List[Point3D], indices: List[IndexTriplet], **props_kw):
    pass


def axis(
    min_vec: Union[Vec2D, Vec3D, List, np.ndarray, Tuple],
    max_vec: Union[Vec2D, Vec3D, List, np.ndarray, Tuple],
):
    min_vec = internal.ensure_vec3d(min_vec, -1.0)
    max_vec = internal.ensure_vec3d(max_vec, 1.0)

    if min_vec.x >= max_vec.x:
        raise Exception("Invalid axis! x min >= x max")
    if min_vec.y >= max_vec.y:
        raise Exception("Invalid axis! y min >= y max")
    if min_vec.z >= max_vec.z:
        raise Exception("Invalid axis! z min >= z max")

    hdr = internal.CommunicationHeader(Function.AXES_3D)
    hdr.append(CommunicationHeaderObjectType.AXIS_MIN_MAX_VEC, (min_vec, max_vec))
    internal.send_header(internal.send_with_tcp, hdr)


def view(azimuth_deg, elevation_deg):
    hdr = internal.CommunicationHeader(Function.VIEW)
    hdr.append(CommunicationHeaderObjectType.AZIMUTH, np.float32(azimuth_deg))
    hdr.append(CommunicationHeaderObjectType.ELEVATION, np.float32(elevation_deg))
    internal.send_header(internal.send_with_tcp, hdr)


def disable_scale_on_rotation():
    hdr = internal.CommunicationHeader(Function.DISABLE_SCALE_ON_ROTATION)
    internal.send_header(internal.send_with_tcp, hdr)


def soft_clear_view():
    hdr = internal.CommunicationHeader(Function.SOFT_CLEAR)
    internal.send_header(internal.send_with_tcp, hdr)


def clear_view():
    hdr = internal.CommunicationHeader(Function.CLEAR)
    internal.send_header(internal.send_with_tcp, hdr)


def set_current_element(name: str):
    if not isinstance(name, str):
        raise Exception("Name must be a string!")

    hdr = internal.CommunicationHeader(Function.SET_CURRENT_ELEMENT)
    hdr.append(CommunicationHeaderObjectType.ELEMENT_NAME, name)
    internal.send_header(internal.send_with_tcp, hdr)
