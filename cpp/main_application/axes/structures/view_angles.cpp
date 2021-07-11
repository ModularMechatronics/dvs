#include "axes/structures/view_angles.h"

ViewAngles::ViewAngles()
    : azimuth_(0.0), snapped_azimuth_(0.0), elevation_(0.0), snapped_elevation_(0.0), angle_limit_(5.0 * M_PI / 180.0)
{
}

ViewAngles::ViewAngles(const double azimuth, const double elevation, const double angle_limit)
    : angle_limit_(angle_limit)
{
    setAngles(azimuth, elevation);
}

ViewAngles::ViewAngles(const double azimuth, const double elevation) : angle_limit_(5.0 * M_PI / 180.0)
{
    setAngles(azimuth, elevation);
}

bool ViewAngles::bothSnappedBelowAngleLimitAroundZero() const
{
    return (std::fabs(getSnappedAzimuth()) < angle_limit_) && (std::fabs(getSnappedElevation()) < angle_limit_);
}

double ViewAngles::getAngleLimit() const
{
    return angle_limit_;
}

void ViewAngles::setSnapAngles()
{
    if (isCloseToSnap())
    {
        snapped_azimuth_ = calcAzimuthSnapAngle();
        snapped_elevation_ = calcElevationSnapAngle();
    }
    else
    {
        snapped_azimuth_ = getAzimuth();
        snapped_elevation_ = getElevation();
    }
}

void ViewAngles::setAngles(const double azimuth, const double elevation)
{
    azimuth_ = std::min(azimuth, M_PI);
    azimuth_ = std::max(azimuth_, -M_PI);
    elevation_ = std::min(elevation, M_PI / 2.0);
    elevation_ = std::max(elevation_, -M_PI / 2.0);
    setSnapAngles();
}

void ViewAngles::changeAnglesWithDelta(const double delta_azimuth, const double delta_elevation)
{
    double new_azimuth;
    double new_elevation;

    if ((getAzimuth() + delta_azimuth) > M_PI)
    {
        new_azimuth = -M_PI + std::fabs((getAzimuth() + delta_azimuth) - M_PI);
    }
    else if ((getAzimuth() + delta_azimuth) < -M_PI)
    {
        new_azimuth = M_PI - std::fabs((getAzimuth() + delta_azimuth) + M_PI);
    }
    else
    {
        new_azimuth = getAzimuth() + delta_azimuth;
    }

    new_elevation = getElevation() + delta_elevation;
    new_elevation = std::min(new_elevation, M_PI / 2.0);
    new_elevation = std::max(new_elevation, -M_PI / 2.0);

    setAngles(new_azimuth, new_elevation);
}

double ViewAngles::getAzimuth() const
{
    return azimuth_;
}

double ViewAngles::getElevation() const
{
    return elevation_;
}

double ViewAngles::getSnappedAzimuth() const
{
    return snapped_azimuth_;
}

bool ViewAngles::isSnappedAlongX() const
{
    const float abs_az = std::abs(azimuth_);
    const float abs_el = std::abs(elevation_);

    return (std::abs(M_PI_2 - abs_az) < angle_limit_) && (abs_el < angle_limit_);
}

bool ViewAngles::isSnappedAlongY() const
{
    const float abs_az = std::abs(azimuth_);
    const float abs_el = std::abs(elevation_);
    return ((abs_az < angle_limit_) || (std::abs(M_PI - abs_az) < angle_limit_)) && (abs_el < angle_limit_);
}

bool ViewAngles::isSnappedAlongZ() const
{
    const float abs_el = std::abs(elevation_);
    return (std::abs(M_PI_2 - abs_el) < angle_limit_);
}

double ViewAngles::getSnappedElevation() const
{
    return snapped_elevation_;
}

AxisAngled ViewAngles::getAngleAxis() const
{
    const Matrixd rotation_mat = rotationMatrixX(getElevation()) * rotationMatrixY(getAzimuth());
    return rotationMatrixToAxisAngle(rotation_mat);
}

Matrixd ViewAngles::getRotationMatrix() const
{
    return rotationMatrixX(getElevation()) * rotationMatrixY(getAzimuth());
}

AxisAngled ViewAngles::getSnappedAngleAxis() const
{
    return rotationMatrixToAxisAngle(getSnappedRotationMatrix());
}

Matrixd ViewAngles::getSnappedRotationMatrix() const
{
    return rotationMatrixX(-M_PI / 2.0) * rotationMatrixX(getSnappedElevation()) * rotationMatrixZ(getSnappedAzimuth());
}

double ViewAngles::calcElevationSnapAngle() const
{
    if (std::fabs(getElevation()) < angle_limit_)
    {
        return 0.0;
    }
    else if (std::fabs(getElevation() - M_PI / 2.0) < angle_limit_)
    {
        return M_PI / 2.0;
    }
    else if (std::fabs(getElevation() + M_PI / 2.0) < angle_limit_)
    {
        return -M_PI / 2.0;
    }
    else
    {
        return getElevation();
    }
}

double ViewAngles::calcAzimuthSnapAngle() const
{
    if (std::fabs(getAzimuth()) < angle_limit_)
    {
        return 0.0;
    }
    else if (std::fabs(getAzimuth() - M_PI / 2.0) < angle_limit_)
    {
        return M_PI / 2.0;
    }
    else if (std::fabs(getAzimuth() - M_PI) < angle_limit_)
    {
        return M_PI;
    }
    else if (std::fabs(getAzimuth() + M_PI / 2.0) < angle_limit_)
    {
        return -M_PI / 2.0;
    }
    else if (std::fabs(getAzimuth() + M_PI) < angle_limit_)
    {
        return -M_PI;
    }
    else
    {
        return getAzimuth();
    }
}

bool ViewAngles::isCloseToSnap() const
{
    const bool c0 = (std::fabs(getElevation()) < angle_limit_) ||
                    (std::fabs(getElevation() - M_PI / 2.0) < angle_limit_) ||
                    (std::fabs(getElevation() + M_PI / 2.0) < angle_limit_);
    const bool c1 = (std::fabs(getAzimuth()) < angle_limit_) || (std::fabs(getAzimuth() - M_PI / 2.0) < angle_limit_) ||
                    (std::fabs(getAzimuth() - M_PI) < angle_limit_) ||
                    (std::fabs(getAzimuth() + M_PI / 2.0) < angle_limit_) ||
                    (std::fabs(getAzimuth() + M_PI) < angle_limit_);

    return c0 && c1;
}
