#ifndef DVS_CYLINDRICAL_CLASS_H_
#define DVS_CYLINDRICAL_CLASS_H_

namespace dvs
{
template <typename T> struct CylindricalCoord
{
    T r;
    T z;
    T phi;

    CylindricalCoord(const T r_, const T z_, const T phi_);
    CylindricalCoord();

    SphericalCoord<T> toSphericalCoord() const;
    Vec3D<T> toVec3D() const;
};

}  // namespace dvs

#endif
