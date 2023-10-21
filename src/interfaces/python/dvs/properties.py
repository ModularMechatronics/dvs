from enum import Enum
import numpy as np
from typing import List, Union, Tuple
from structures import *
from enums import *
import item_id
from item_id_constants import *

"""
PROPERTY_FLAG,
ITEM_ID,
UNKNOWN

"""


class PointSize:
    def __init__(self, point_size: float):
        if not isinstance(point_size, float):
            raise TypeError("point_size must be of type float!")
        if point_size <= 0.0:
            raise ValueError("point_size must be greater than 0.0!")

        self.point_size = point_size


class LineWidth:
    def __init__(self, line_width: float):
        if not isinstance(line_width, float):
            raise TypeError("line_width must be of type float!")
        if line_width <= 0.0:
            raise ValueError("line_width must be greater than 0.0!")

        self.line_width = line_width


class BufferSize:
    def __init__(self, buffer_size: int):
        if not isinstance(buffer_size, int):
            raise TypeError("buffer_size must be of type int!")
        if buffer_size <= 0:
            raise ValueError("buffer_size must be greater than 0!")

        self.buffer_size = buffer_size


# TODO: Move to enums.py
class DistanceFromType(Enum):
    X = 0
    Y = 1
    Z = 2
    XY = 3
    XZ = 4
    YZ = 5
    XYZ = 6


class DistanceFrom:
    def __init__(
        self,
        pt: Vec3D,
        min_dist: float,
        max_dist: float,
        distance_from_type: DistanceFromType,
    ):
        self.pt = pt
        self.min_distance = min_dist
        self.max_distance = max_dist
        self.distance_from_type = distance_from_type

    @staticmethod
    def x(x_val: float, min_dist: float, max_dist: float):
        return DistanceFrom(
            Vec3D(x_val, 0.0, 0.0), min_dist, max_dist, DistanceFromType.X
        )

    @staticmethod
    def y(y_val: float, min_dist: float, max_dist: float):
        return DistanceFrom(
            Vec3D(0.0, y_val, 0.0), min_dist, max_dist, DistanceFromType.Y
        )

    @staticmethod
    def z(z_val: float, min_dist: float, max_dist: float):
        return DistanceFrom(
            Vec3D(0.0, 0.0, z_val), min_dist, max_dist, DistanceFromType.Z
        )

    @staticmethod
    def xy(p: PointXY, min_dist: float, max_dist: float):
        return DistanceFrom(
            Vec3D(p.x, p.y, 0.0), min_dist, max_dist, DistanceFromType.XY
        )

    @staticmethod
    def xz(p: PointXZ, min_dist: float, max_dist: float):
        return DistanceFrom(
            Vec3D(p.x, 0.0, p.z), min_dist, max_dist, DistanceFromType.XZ
        )

    @staticmethod
    def yz(p: PointYZ, min_dist: float, max_dist: float):
        return DistanceFrom(
            Vec3D(0.0, p.y, p.z), min_dist, max_dist, DistanceFromType.YZ
        )

    @staticmethod
    def xyz(p: Point3D, min_dist: float, max_dist: float):
        return DistanceFrom(p, min_dist, max_dist, DistanceFromType.XYZ)


class Name:
    def __init__(self, name: str):
        if not isinstance(name, str):
            raise TypeError("Input name must be of type str")
        self.name = name


class Transform:
    def __init__(
        self,
        scale_matrix: np.ndarray,
        rotation_matrix: np.ndarray,
        translation_vector: np.ndarray,
    ):
        if not isinstance(scale_matrix, np.ndarray):
            raise TypeError("scale_matrix must be of type numpy.ndarray")
        if not isinstance(translation_vector, np.ndarray):
            raise TypeError("translation_vector must be of type numpy.ndarray")
        if not isinstance(rotation_matrix, np.ndarray):
            raise TypeError("rotation_matrix must be of type numpy.ndarray")

        if scale_matrix.shape != (3, 3):
            raise ValueError("scale_matrix must be of shape (3,3)")
        if (
            translation_vector.shape != (3,)
            and translation_vector.shape != (3, 1)
            and translation_vector.shape != (1, 3)
        ):
            raise ValueError(
                "translation_vector must be of shape (3,), (3,1) or (1,3)!"
            )
        if rotation_matrix.shape != (3, 3):
            raise ValueError("rotation_matrix must be of shape (3,3)")

        self.scale_matrix = scale_matrix
        self.translation_vector = translation_vector.flatten()
        self.rotation_matrix = rotation_matrix


class ZOffset:
    """
    Represents an offset from the z=0 plane in three-dimensional space.
    """

    def __init__(self, z_offset: float):
        """
        Initializes a ZOffset object with the specified offset value.

        Args:
            z_offset (float): The offset value from the z=0 plane.
        """
        if not isinstance(z_offset, float):
            raise TypeError("z_offset must be of type float!")
        self.z_offset = z_offset


class Alpha:
    """
    Represents alpha transparency value.

    Attributes:
        alpha (float): Alpha transparency value ranging from 0.0 (completely transparent) to 1.0 (completely opaque).

    Methods:
        __init__(self, alpha: float):
            Initializes an Alpha object with the specified transparency value.
    """

    def __init__(self, alpha: float):
        """
        Initializes an Alpha object with the specified transparency value.

        Args:
            alpha (float): Alpha transparency value ranging from 0.0 (completely transparent) to 1.0 (completely opaque).
        """
        self.alpha = alpha


# TODO: Move to properties_internals.py or something
class ColorT(Enum):
    RED = 0
    GREEN = 1
    BLUE = 2
    CYAN = 3
    MAGENTA = 4
    YELLOW = 5
    BLACK = 6
    WHITE = 7
    GRAY = 8


class EdgeColorT(Enum):
    RED = 0
    GREEN = 1
    BLUE = 2
    CYAN = 3
    MAGENTA = 4
    YELLOW = 5
    BLACK = 6
    WHITE = 7
    GRAY = 8


class FaceColorT(Enum):
    RED = 0
    GREEN = 1
    BLUE = 2
    CYAN = 3
    MAGENTA = 4
    YELLOW = 5
    BLACK = 6
    WHITE = 7
    GRAY = 8


class Color:
    """
    Represents a color using floating point RGB components.

    Methods:
        _convert_to_valid_type_and_range(color_component):
            Static method to convert a color component to a valid type and range.

        __init__(self, r: float, g: float, b: float):
            Constructor method to initialize a Color object with RGB components.

    """

    RED = ColorT.RED
    GREEN = ColorT.GREEN
    BLUE = ColorT.BLUE
    CYAN = ColorT.CYAN
    MAGENTA = ColorT.MAGENTA
    YELLOW = ColorT.YELLOW
    BLACK = ColorT.BLACK
    WHITE = ColorT.WHITE
    GRAY = ColorT.GRAY

    @staticmethod
    def _convert_to_valid_type_and_range(color_component):
        return np.round(np.float32(color_component) * 255.0).astype(np.uint8)

    def __init__(self, r: float, g: float, b: float):
        """
        Initialize a Color object with RGB components.

        Args:
            r (float): Red component value (0.0 to 1.0).
            g (float): Green component value (0.0 to 1.0).
            b (float): Blue component value (0.0 to 1.0).
        """
        self.r = r
        self.g = g
        self.b = b


class FaceColor(Color):
    RED = FaceColorT.RED
    GREEN = FaceColorT.GREEN
    BLUE = FaceColorT.BLUE
    CYAN = FaceColorT.CYAN
    MAGENTA = FaceColorT.MAGENTA
    YELLOW = FaceColorT.YELLOW
    BLACK = FaceColorT.BLACK
    WHITE = FaceColorT.WHITE
    GRAY = FaceColorT.GRAY

    def __init__(self, r: float, g: float, b: float):
        """
        Initialize a FaceColor object with RGB components.

        Args:
            r (float): Red component value (0.0 to 1.0).
            g (float): Green component value (0.0 to 1.0).
            b (float): Blue component value (0.0 to 1.0).
        """
        super().__init__(r, g, b)


class EdgeColor(Color):
    RED = EdgeColorT.RED
    GREEN = EdgeColorT.GREEN
    BLUE = EdgeColorT.BLUE
    CYAN = EdgeColorT.CYAN
    MAGENTA = EdgeColorT.MAGENTA
    YELLOW = EdgeColorT.YELLOW
    BLACK = EdgeColorT.BLACK
    WHITE = EdgeColorT.WHITE
    GRAY = EdgeColorT.GRAY

    def __init__(self, r: float, g: float, b: float):
        """
        Initialize a EdgeColor object with RGB components.

        Args:
            r (float): Red component value (0.0 to 1.0).
            g (float): Green component value (0.0 to 1.0).
            b (float): Blue component value (0.0 to 1.0).
        """
        super().__init__(r, g, b)


class ScatterStyle(Enum):
    SQUARE = 0
    CIRCLE = 1
    DISC = 2
    PLUS = 3
    CROSS = 4


class ColorMap(Enum):
    JET = 0
    HSV = 1
    MAGMA = 2
    VIRIDIS = 3
    PASTEL = 4
    JET_SOFT = 5
    JET_BRIGHT = 6
    UNKNOWN = 7


class LineStyle(Enum):
    SOLID = 0
    DASHED = 1
    SHORT_DASHED = 2
    LONG_DASHED = 3


PERSISTENT = PropertyFlag.PERSISTENT
INTERPOLATE_COLORMAP = PropertyFlag.INTERPOLATE_COLORMAP
FAST_PLOT = PropertyFlag.FAST_PLOT
APPENDABLE = PropertyFlag.APPENDABLE
EXCLUDE_FROM_SELECTION = PropertyFlag.EXCLUDE_FROM_SELECTION
# TODO: Add UPDATABLE = PropertyFlag.UPDATABLE

PROPERTY_TYPE_TO_CLASS = {
    PropertyType.LINE_WIDTH: LineWidth,
    PropertyType.POINT_SIZE: PointSize,
    PropertyType.ALPHA: Alpha,
    PropertyType.NAME: Name,
    PropertyType.BUFFER_SIZE: BufferSize,
    PropertyType.Z_OFFSET: ZOffset,
}
