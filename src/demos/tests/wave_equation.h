#ifndef DEMOS_TESTS_WAVE_EQUATION_H_
#define DEMOS_TESTS_WAVE_EQUATION_H_

#include "dvs/dvs.h"

using namespace dvs;

namespace wave_equation
{

struct WaveEquationParams
{
    float a;
    float b;
    float c;

    float dx;
    float dy;
    float dt;

    WaveEquationParams() = default;
    WaveEquationParams(const float c_, const float dx_, const float dy_, const float dt_)
    {
        dx = dx_;
        dy = dy_;
        dt = dt_;

        c = c_;

        a = c_ * c_ * dt_ / (dx_ * dx_);
        b = 2.0f * 2.0f * dt_ / (dy_ * dy_);

        if ((a > 0.5f) || (b > 1.0f))
        {
            std::cout << "Stability not ensured!" << std::endl;
            std::cout << "a" << a << std::endl;
            std::cout << "b" << b << std::endl;
        }
    }
};

class WaveEquationStepper
{
private:
    Matrix<float> m0_;
    Matrix<float> m1_;
    Matrix<float> m2_;
    const WaveEquationParams params_;
    size_t num_rows_;
    size_t num_cols_;
    int it_;

public:
    WaveEquationStepper() = delete;
    WaveEquationStepper(const size_t num_rows, const size_t num_cols, const WaveEquationParams& params)
        : m0_{num_rows, num_cols},
          m1_{num_rows, num_cols},
          m2_{num_rows, num_cols},
          params_{params},
          num_rows_{num_rows},
          num_cols_{num_cols},
          it_{0}
    {
        m0_.fill(0.0f);
        m1_.fill(0.0f);
        m2_.fill(0.0f);

        /*for (size_t r = 20; r < 30; r++)
        {
            for (size_t c = 20; c < 30; c++)
            {
                m0_(r, c) = 0.1f;
            }
        }*/
        m0_(2, 2) = 0.1f;
        m0_(3, 3) = 0.1f;
    }

    void run()
    {
        const float a = params_.a;
        const float b = params_.b;

        const float damping = 0.01f;

        MatrixView<float> prev;
        MatrixView<float> curr;
        MatrixView<float> next;

        if (it_ == 0)
        {
            prev = m0_.view();
            curr = m1_.view();
            next = m2_.view();
        }
        else if (it_ == 1)
        {
            prev = m1_.view();
            curr = m2_.view();
            next = m0_.view();
        }
        else
        {
            prev = m2_.view();
            curr = m0_.view();
            next = m1_.view();
        }

        for (size_t r = 1; r < (num_rows_ - 1); r++)
        {
            for (size_t c = 1; c < (num_cols_ - 1); c++)
            {
                next(r, c) = -damping * (curr(r, c) - prev(r, c)) + 2.0f * curr(r, c) - prev(r, c) +
                             a * (curr(r + 1U, c) - 2.0f * curr(r, c) + curr(r - 1U, c)) +
                             b * (curr(r, c + 1U) - 2.0f * curr(r, c) + curr(r, c - 1U));
            }
        }

        it_++;
        if (it_ == 3)
        {
            it_ = 0;
        }
    }

    MatrixConstView<float> getCurr()
    {
        MatrixConstView<float> curr;

        if (it_ == 0)
        {
            curr = m1_.constView();
        }
        else if (it_ == 1)
        {
            curr = m2_.constView();
        }
        else
        {
            curr = m0_.constView();
        }

        return curr;
    }
};

void testBasic()
{
    const size_t num_rows = 200, num_cols = 200, num_its = 1000;
    const float c = 2.0f, dx = 0.005f, dy = 0.005f, dt = 0.000001f;

    const Vector<float> xv = linspaceFromStartPointIncAndCount<float>(0.0f, dx, num_cols);
    const Vector<float> yv = linspaceFromStartPointIncAndCount<float>(0.0f, dy, num_rows);

    Matrix<float> x, y;
    std::tie(x, y) = meshGrid(xv, yv);

    const WaveEquationParams params{c, dx, dy, dt};

    WaveEquationStepper stepper{num_rows, num_cols, params};

    setCurrentElement("p_view_0");
    clearView();
    // globalIllumination({1.0, 1.0, 1.0});

    const double z_max = 0.05;
    axis({0.0, 0.0, -z_max}, {0.25 * 4.0, 0.25 * 4.0, z_max});
    axis({0.0, 0.0, -z_max}, {0.25 * 0.4, 0.25 * 0.4, z_max});
    view(-14, 40);

    for (size_t k = 0; k < num_its; k++)
    {
        stepper.run();
        const MatrixConstView<float> z = stepper.getCurr();

        surf(x.constView(),
             y.constView(),
             z,
             properties::ColorMap::Jet(),
             properties::EdgeColor::None(),
             properties::INTERPOLATE_COLORMAP);
        std::cin.ignore();
        // usleep(50 * 1000);
        softClearView();
    }
}

}  // namespace wave_equation

#endif  // DEMOS_TESTS_WAVE_EQUATION_H_
