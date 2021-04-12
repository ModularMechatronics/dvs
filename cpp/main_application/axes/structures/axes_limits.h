#ifndef AXES_LIMITS_H_
#define AXES_LIMITS_H_

#include "math/math.h"

using namespace dvs;

class AxesLimits
{
private:
    Vec3Dd lim_min_;
    Vec3Dd lim_max_;
    Vec3Dd tick_begin_;

public:
    AxesLimits() = default;
    AxesLimits(const Vec3Dd& min_vec, const Vec3Dd& max_vec);

    void setTickBegin(const Vec3Dd& tick_begin_vec);

    void setMin(const Vec3Dd& min_vec);
    void setMax(const Vec3Dd& max_vec);

    void incrementMinMax(const Vec3Dd& dv);

    Vec3Dd getMin() const;
    Vec3Dd getMax() const;

    Vec3Dd getTickBegin() const;

    Vec3Dd getAxesCenter() const;

    Vec3Dd getAxesScale() const;
};

#endif
