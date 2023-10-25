import numpy as np
from typing import List, Union, Tuple
import psutil
import os

from enums import *
from serialization import *
from structures import *
import internal
import properties


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


def set_properties(item_id: item_id.ItemId, **props_kw):
    hdr = internal.CommunicationHeader(Function.PROPERTIES_EXTENSION)
    hdr.append(CommunicationHeaderObjectType.ITEM_ID, item_id)
    hdr.append_properties(props_kw)
    internal.send_header(internal.send_with_tcp, hdr)


def delete_plot_object(item_id: item_id.ItemId):
    hdr = internal.CommunicationHeader(Function.DELETE_PLOT_OBJECT)
    hdr.append(CommunicationHeaderObjectType.ITEM_ID, item_id)
    internal.send_header(internal.send_with_tcp, hdr)


def set_current_element_to_image_view():
    hdr = internal.CommunicationHeader(Function.CURRENT_ELEMENT_AS_IMAGE_VIEW)
    internal.send_header(internal.send_with_tcp, hdr)


def wait_for_flush():
    hdr = internal.CommunicationHeader(Function.WAIT_FOR_FLUSH)
    internal.send_header(internal.send_with_tcp, hdr)


def flush_current_element():
    hdr = internal.CommunicationHeader(Function.FLUSH_ELEMENT)
    internal.send_header(internal.send_with_tcp, hdr)


def flush_multiple_elements(*args):
    hdr = internal.CommunicationHeader(Function.FLUSH_MULTIPLE_ELEMENTS)

    name_lengths = np.zeros(len(args), dtype=np.uint8)
    concatenated_names = ""

    for idx, arg in enumerate(args):
        if not isinstance(arg, str):
            raise Exception("All arguments must be strings!")

        name_lengths[idx] = len(arg)
        concatenated_names += arg

    hdr.append(CommunicationHeaderObjectType.NUM_NAMES, len(args))
    internal.send_header_and_data(
        internal.send_with_tcp, hdr, name_lengths, concatenated_names
    )


def global_illumination(light_position: Vec3D):
    hdr = internal.CommunicationHeader(Function.GLOBAL_ILLUMINATION)
    hdr.append(CommunicationHeaderObjectType.VEC3, light_position)
    internal.send_header(internal.send_with_tcp, hdr)


def show_legend():
    hdr = internal.CommunicationHeader(Function.SHOW_LEGEND)
    internal.send_header(internal.send_with_tcp, hdr)


def disable_automatic_axes_adjustment():
    hdr = internal.CommunicationHeader(Function.DISABLE_AXES_FROM_MIN_MAX)
    internal.send_header(internal.send_with_tcp, hdr)


def axes_square():
    hdr = internal.CommunicationHeader(Function.AXES_SQUARE)
    internal.send_header(internal.send_with_tcp, hdr)


def set_axes_box_scale_factor(scale_vector: Vec3D):
    hdr = internal.CommunicationHeader(Function.SET_AXES_BOX_SCALE_FACTOR)
    hdr.append(CommunicationHeaderObjectType.VEC3, scale_vector)
    internal.send_header(internal.send_with_tcp, hdr)


def set_title(title: str):
    if not isinstance(title, str):
        raise Exception("Title must be a string!")
    elif len(title) == 0:
        raise Exception("Title must have length greater than 0!")

    hdr = internal.CommunicationHeader(Function.SET_TITLE)
    hdr.append(CommunicationHeaderObjectType.TITLE_STRING, title)
    internal.send_header(internal.send_with_tcp, hdr)


def set_transform(
    item_id: item_id.ItemId,
    scale_matrix: np.ndarray,
    rotation_matrix: np.ndarray,
    translation_vec: Union[np.ndarray, Vec3D],
):
    if not isinstance(item_id, item_id.ItemId):
        raise Exception("Item ID must be an ItemId!")

    if isinstance(translation_vec, Vec3D):
        translation_vec = translation_vec
    elif not isinstance(translation_vec, np.ndarray):
        if translation_vec.shape == (3,):
            translation_vec = Vec3D(
                translation_vec[0], translation_vec[1], translation_vec[2]
            )
        elif translation_vec.shape == (1, 3):
            translation_vec = Vec3D(
                translation_vec[0][0], translation_vec[0][1], translation_vec[0][2]
            )
        elif translation_vec.shape == (3, 1):
            translation_vec = Vec3D(
                translation_vec[0][0], translation_vec[1][0], translation_vec[2][0]
            )
        else:
            raise Exception("Translation vector must be a Vec3D or a numpy array!")
    else:
        raise Exception("Translation vector must be a Vec3D or a numpy array!")

    if not scale_matrix.shape == (3, 3):
        raise Exception("Scale matrix must be a 3x3 matrix!")
    if not rotation_matrix.shape == (3, 3):
        raise Exception("Rotation matrix must be a 3x3 matrix!")

    hdr = internal.CommunicationHeader(Function.SET_OBJECT_TRANSFORM)
    hdr.append(CommunicationHeaderObjectType.ROTATION_MATRIX, rotation_matrix)
    hdr.append(CommunicationHeaderObjectType.TRANSLATION_VECTOR, translation_vec)
    hdr.append(CommunicationHeaderObjectType.SCALE_MATRIX, scale_matrix)
    hdr.append(CommunicationHeaderObjectType.ITEM_ID, item_id)
    internal.send_header(internal.send_with_tcp, hdr)


def open_project_file(file_path: str):
    if not isinstance(file_path, str):
        raise Exception("File path must be a string!")
    elif len(file_path) == 0:
        raise Exception("File path must have length greater than 0!")

    hdr = internal.CommunicationHeader(Function.OPEN_PROJECT_FILE)
    hdr.append(CommunicationHeaderObjectType.PROJECT_FILE_NAME, file_path)
    internal.send_header(internal.send_with_tcp, hdr)


def spawn():
    # TODO: This function does not work...
    return
    if "dvs" not in [p.name() for p in psutil.process_iter()]:
        os.spawnl(
            os.P_NOWAIT,
            "/Users/danielpi/work/dvs/src/build/main_application/dvs",
            "/Users/danielpi/work/dvs/src/build/",
        )
