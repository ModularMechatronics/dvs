#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "code.h"

class Test0 : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

#define ROTATION_60 (M_PI / 3.0)
#define ROTATION_120 (2.0 * ROTATION_60)
#define ROTATION_180 (M_PI)
#define ROTATION_240 (ROTATION_180 + ROTATION_60)
#define ROTATION_300 (ROTATION_240 + ROTATION_60)
#define ROTATION_360 (2.0 * M_PI)

uint8_t findSegmentGTFunction(const double wrapped_angle)
{
    if(wrapped_angle < 0.0)
    {
        throw std::runtime_error("Got negative angle!");
    }
    else if(wrapped_angle > (2.0 * M_PI))
    {
        throw std::runtime_error("Got angle greated than 2*pi!");
    }
    else if(wrapped_angle < ROTATION_60)
    {
        return 0;
    }
    else if(wrapped_angle < ROTATION_120)
    {
        return 1;
    }
    else if(wrapped_angle < ROTATION_180)
    {
        return 2;
    }
    else if(wrapped_angle < ROTATION_240)
    {
        return 3;
    }
    else if(wrapped_angle < ROTATION_300)
    {
        return 5;
    }
    else if(wrapped_angle < ROTATION_360)
    {
        return 6;
    }
    else
    {
        return 255; // Won't come here
    }
}

constexpr double kEps = 0.0000001;

TEST_F(Test0, TestBasic)
{
    const double dphi = 0.001;
    const double pos_phi = 2.0 * M_PI + dphi;
    const double neg_phi = -dphi;

    ASSERT_NEAR(dphi, wrapAngle(pos_phi), kEps);
    ASSERT_NEAR(2.0 * M_PI - dphi, wrapAngle(neg_phi), kEps);
}

/*struct IAlphaBeta
{
    double alpha;
    double beta;
};

struct IDirQuad
{
    double d;
    double q;
};

{
    virtual_motor.vd =  virtual_motor.cos_phi * v_alpha + virtual_motor.sin_phi * v_beta;
	virtual_motor.vq =  virtual_motor.cos_phi * v_beta - virtual_motor.sin_phi * v_alpha;
}

void parkInverse(const IDirQuad* idq, const double cos_phi, const double sin_phi, IAlphaBeta* iab)
{
    iab->alpha = cos_phi * idq->d - sin_phi * idq->q;
	iab->beta  = cos_phi * idq->q + sin_phi * idq->d;

	virtual_motor.v_alpha = cos_phi * virtual_motor.vd -
							sin_phi * virtual_motor.vq;
	virtual_motor.v_beta  = cos_phi * virtual_motor.vq +
							sin_phi * virtual_motor.vd;
}

{
    //	Park Inverse
	virtual_motor.i_alpha = virtual_motor.cos_phi * virtual_motor.id -
							virtual_motor.sin_phi * virtual_motor.iq;
	virtual_motor.i_beta  = virtual_motor.cos_phi * virtual_motor.iq +
							virtual_motor.sin_phi * virtual_motor.id;

	virtual_motor.v_alpha = virtual_motor.cos_phi * virtual_motor.vd -
							virtual_motor.sin_phi * virtual_motor.vq;
	virtual_motor.v_beta  = virtual_motor.cos_phi * virtual_motor.vq +
							virtual_motor.sin_phi * virtual_motor.vd;

	//	Clark Inverse
	virtual_motor.ia = virtual_motor.i_alpha;
	virtual_motor.ib = -0.5 * virtual_motor.i_alpha + SQRT3_BY_2 * virtual_motor.i_beta;
	virtual_motor.ic = -0.5 * virtual_motor.i_alpha - SQRT3_BY_2 * virtual_motor.i_beta;

	virtual_motor.va = virtual_motor.v_alpha;
	virtual_motor.vb = -0.5 * virtual_motor.v_alpha + SQRT3_BY_2 * virtual_motor.v_beta;
	virtual_motor.vc = -0.5 * virtual_motor.v_alpha - SQRT3_BY_2 * virtual_motor.v_beta;
}
*/
TEST_F(Test0, TestBasic1)
{
    const double dphi = 0.001;
    const double pos_phi = 2.0 * M_PI + dphi;
    const double neg_phi = -dphi;

    ASSERT_NEAR(dphi, wrapAngle(pos_phi), kEps);
    ASSERT_NEAR(2.0 * M_PI - dphi, wrapAngle(neg_phi), kEps);
}
