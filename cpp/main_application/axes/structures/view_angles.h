#ifndef VIEW_ANGLES_H_
#define VIEW_ANGLES_H_

#include "dvs/math/math.h"

using namespace dvs;

class ViewAngles
{
private:
    double azimuth_;
    double snapped_azimuth_;
    double elevation_;
    double snapped_elevation_;
    double angle_limit_;

    void setSnapAngles();
    double calcElevationSnapAngle() const;
    double calcAzimuthSnapAngle() const;

public:
    ViewAngles();
    ViewAngles(const double azimuth, const double elevation);
    ViewAngles(const double azimuth, const double elevation, const double angle_limit);
    void setAngles(const double azimuth, const double elevation);
    void changeAnglesWithDelta(const double azimuth, const double elevation);
    double getAzimuth() const;
    double getElevation() const;
    double getSnappedAzimuth() const;
    double getSnappedElevation() const;
    AxisAngled getAngleAxis() const;
    Matrixd getRotationMatrix() const;
    AxisAngled getSnappedAngleAxis() const;
    Matrixd getSnappedRotationMatrix() const;
    bool isCloseToSnap() const;
    bool bothSnappedBelowAngleLimitAroundZero() const;
    double getAngleLimit() const;

    bool isSnappedAlongX() const;
    bool isSnappedAlongY() const;
    bool isSnappedAlongZ() const;
};

#endif
