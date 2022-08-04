#ifndef _CODE_H_
#define _CODE_H_

#define PI_BY_3 1.0471975511965977461542144610931676280657231331250352736583148641

struct PidParameters
{
    double kp;
    double ki;
    double kd;
};

double mapValue(const double val,
                const double from_min,
                const double from_max,
                const double to_min,
                const double to_max)
{
    return (to_max - to_min) * (val - from_min) / (from_max - from_min) + to_min;
}

double truncateNumberDouble(const double val, const double min_val, const double max_val)
{
    if(val < min_val)
    {
        return min_val;
    }
    else if(val > max_val)
    {
        return max_val;
    }
    else
    {
        return val;
    }
}

int64_t truncateNumberInt64(const int64_t val, const int64_t min_val, const int64_t max_val)
{
    if(val < min_val)
    {
        return min_val;
    }
    else if(val > max_val)
    {
        return max_val;
    }
    else
    {
        return val;
    }
}

double wrapAngle(const double angle)
{
    double wrapped_angle = fmod(angle, M_PI * 2.0);

    if (wrapped_angle < 0.0)
    {
        wrapped_angle += (M_PI * 2.0);
    }

    return wrapped_angle;
}

uint8_t findSegment(const double wrapped_angle)
{
    // Assumes wrapped_angle is in the interval [0, 2 * pi]
    const double segment = wrapped_angle / PI_BY_3;
    return segment;
}

struct PhasePercentage
{
    double a;
    double b;
    double c;
};

void calculatePhasePercentage(const double elec_angle, PhasePercentage* pp)
{
    const uint8_t segment = findSegment(elec_angle);
    const double angle_in_segment = elec_angle - ((double)segment) * PI_BY_3;

    // pp->a = ...;
    // pp->b = ...;
    // pp->c = ...;
}

#endif