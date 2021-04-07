#ifndef DVS_SPHERICAL_CLASS_H_
#define DVS_SPHERICAL_CLASS_H_

namespace dvs
{
template <typename T> struct SphericalCoord
{
    // phi for azimuth, theta for inclination
    T r;
    T phi;
    T theta;

    SphericalCoord(const T r_, const T phi_, const T theta_);
    SphericalCoord();

    CylindricalCoord<T> toCylindricalVec() const;
    Vec3D<T> toVec3D() const;
};

}  // namespace dvs

#endif
