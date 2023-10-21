# TODO: Remove 'D' from class names and remove default initialization values
class Vec2D:
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y


class Vec3D:
    def __init__(self, x=0, y=0, z=0):
        self.x = x
        self.y = y
        self.z = z


class Vec4D:
    def __init__(self, x=0, y=0, z=0, w=0):
        self.x = x
        self.y = y
        self.z = z
        self.w = w


class VecXY:
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y


class VecXZ:
    def __init__(self, x=0, z=0):
        self.x = x
        self.z = z


class VecYZ:
    def __init__(self, y=0, z=0):
        self.y = y
        self.z = z


Point2D = Vec2D
Point3D = Vec3D
Point4D = Vec4D

PointXY = VecXY
PointXZ = VecXZ
PointYZ = VecYZ


class Line3D:
    def __init__(self, p=Point3D(), v=Vec3D()):
        self.p = p
        self.v = v


class PLine2D:
    def __init__(self, p=Point2D(), v=Vec2D()):
        self.p = p
        self.v = v


class HLine2D:
    def __init__(self, a=0, b=0, c=0):
        self.a = a
        self.b = b
        self.c = c


class Plane:
    def __init__(self, a=0, b=0, c=0, d=0):
        self.a = a
        self.b = b
        self.c = c
        self.d = d


class Triangle3D:
    def __init__(self, p0=Point3D(), p1=Point3D(), p2=Point3D()):
        self.p0 = p0
        self.p1 = p1
        self.p2 = p2


class IndexTriplet:
    def __init__(self, i1=0, i0=0, i2=0):
        self.i0 = i0
        self.i1 = i1
        self.i2 = i2
