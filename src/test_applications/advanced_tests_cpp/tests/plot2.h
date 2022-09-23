#ifndef TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_PLOT2_H_
#define TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_PLOT2_H_

#include "dvs/dvs.h"

using namespace dvs;

namespace plot2
{

void testWithVectorView()
{
    const size_t num_elements = 10;
    const Vector<double> x = linspaceFromBoundariesAndCount<float>(0.0f, 5.0f, num_elements);
    const Vector<double> y = dvs::sin(x);

    const VectorView<double> xv = x.view();
    const VectorView<double> yv = y.view();

    setCurrentElement("view_00");
    clearView();
    scatter(x, y, properties::Color::Red());
    plot(xv, yv, properties::Color::Blue());
}

struct KleinParams
{
    struct XParams
    {
        float a; // Stretch in x direction
        float b;
        float c;
        float d;
        float e;
        float f;
    };
    struct YParams
    {
        float a;
        float b;
        float c;
        float d;
        float e;
        float f;
        float g;
        float h;
        float i;
        float j;
    };
    struct ZParams
    {
        float a;
        float b;
        float c;
    };

    XParams x;
    YParams y;
    ZParams z;
    
    KleinParams()
    {
        x.a = -2.0f/15.0f;
        x.b = 3.0f;
        x.c = 30.0f;
        x.d = 90.0f;
        x.e = 60.0f;
        x.f = 5.0f;

        y.a = -1.0/15.0f;
        y.b = 3.0f;
        y.c = 3.0f;
        y.d = 48.0f;
        y.e = 48.0f;
        y.f = 60.0f;
        y.g = 5.0f;
        y.h = 5.0f;
        y.i = 80.0f;
        y.j = 80.0f;

        z.a = -2.0/15.0f;
        z.b = 3.0f;
        z.c = 5.0f;
    }

};

void evalKlein(const Matrix<float>& u, const Matrix<float>& v, Matrix<float>& x, Matrix<float>& y, Matrix<float>& z, const float u_offset, const float v_offset, const KleinParams& klein_params)
{
    const size_t nu = u.numRows();
    const size_t nv = u.numCols();

    const KleinParams& kp = klein_params;

    for(size_t ku = 0; ku < nu; ku++)
    {
        for(size_t kv = 0; kv < nv; kv++)
        {
            const float u_val = u(ku, kv) + u_offset;
            const float v_val = v(ku, kv) + v_offset;

            const float cos_u = std::cos(u_val);
            const float sin_u = std::sin(u_val);
            const float cos_v = std::cos(v_val);
            const float sin_v = std::sin(v_val);

            const float cos2_u = cos_u * cos_u;
            const float cos3_u = cos2_u * cos_u;
            const float cos4_u = cos3_u * cos_u;
            const float cos5_u = cos4_u * cos_u;
            const float cos6_u = cos5_u * cos_u;
            const float cos7_u = cos6_u * cos_u;

            x(ku, kv) = kp.x.a * cos_u * (kp.x.b * cos_v - kp.x.c * sin_u +
                kp.x.d * cos4_u * sin_u - kp.x.e * cos6_u * sin_u + kp.x.f * cos_u * cos_v * sin_u);

            y(ku, kv) = kp.y.a * sin_u * (kp.y.b * cos_v - kp.y.c * cos2_u * cos_v -
                kp.y.d * cos4_u * cos_v + kp.y.e * cos6_u * cos_v - kp.y.f * sin_u +
                kp.y.g * cos_u * cos_v * sin_u - kp.y.h * cos3_u * cos_v * sin_u - 
                kp.y.i * cos5_u * cos_v * sin_u + kp.y.j * cos7_u * cos_v * sin_u);

            z(ku, kv) = kp.z.a * (kp.z.b + kp.z.c * cos_u * sin_u) * sin_v;
        }
    }
}

void Klein_testBasic()
{
    const size_t nu = 50, nv = 50;

    std::pair<Matrix<float>, Matrix<float>> uv_mats = meshGrid<float>(0.0f, M_PI, 0.0f, 2.0f * M_PI, nv, nu);

    Matrix<float> x(nu, nv), y(nu, nv), z(nu, nv);

    Matrix<float>& u_mat = uv_mats.first;
    Matrix<float>& v_mat = uv_mats.second;

    KleinParams klein_params{};

    VectorView<float> vx(x.data(), nu * nv);
    VectorView<float> vy(y.data(), nu * nv);
    VectorView<float> vz(z.data(), nu * nv);

    setCurrentElement("view_00");
    clearView();

    evalKlein(u_mat, v_mat, x, y, z, 0.0f, 0.0f, klein_params);

    scatter3(vx, vy, vz, properties::Color::Red());
    plot3(vx, vy, vz, properties::Color::Blue());
}

void Klein_testAdvanced0()
{
    const size_t nu = 50, nv = 50;
    const size_t num_its = 10;

    std::pair<Matrix<float>, Matrix<float>> uv_mats = meshGrid<float>(0.0f, M_PI, 0.0f, 2.0f * M_PI, nv, nu);

    Matrix<float> x(nu, nv), y(nu, nv), z(nu, nv);

    Matrix<float>& u_mat = uv_mats.first;
    Matrix<float>& v_mat = uv_mats.second;

    KleinParams klein_params{};

    VectorView<float> vx(x.data(), nu * nv);
    VectorView<float> vy(y.data(), nu * nv);
    VectorView<float> vz(z.data(), nu * nv);

    setCurrentElement("view_00");
    clearView();
    float u_offset = 0.0f, v_offset = 0.0f;

    for(size_t k = 0; k < num_its; k++)
    {
        evalKlein(u_mat, v_mat, x, y, z, u_offset, v_offset, klein_params);
        u_offset += 0.1;

        scatter3(vx, vy, vz, properties::Color::Red());
        // plot3(vx, vy, vz, properties::Color::Blue());
        // axis({-130.0, 0.0, -1.0}, {110.0, 4.0, 1.0});

        usleep(50000);

        softClearView();
    }    
    
    
}

void Klein_testAdvanced1()
{
    const size_t nu = 50, nv = 50;
    const size_t num_triangles = nu * nv * 2;
    const size_t num_its = 100;

    Vector<IndexTriplet> indices(num_triangles);

    std::pair<Matrix<float>, Matrix<float>> uv_mats = meshGrid<float>(0.0f, M_PI, 0.0f, 2.0f * M_PI, nv, nu);

    Matrix<float> x(nu, nv), y(nu, nv), z(nu, nv);

    Matrix<float>& u_mat = uv_mats.first;
    Matrix<float>& v_mat = uv_mats.second;

    KleinParams klein_params{};

    VectorView<float> vx(x.data(), nu * nv);
    VectorView<float> vy(y.data(), nu * nv);
    VectorView<float> vz(z.data(), nu * nv);

    size_t idx = 0U;
    for(size_t ku = 0; ku < (nu - 1U); ku++)
    {
        for(size_t kv = 0; kv < (nv - 1U); kv++)
        {
            const size_t idx2 = ku * nv + kv;
            indices(idx).i0 = idx2;
            indices(idx).i1 = idx2 + 1;
            indices(idx).i2 = idx2 + nv;

            idx += 1;

            indices(idx).i0 = idx2 + nv + 1;
            indices(idx).i1 = idx2 + 1;
            indices(idx).i2 = idx2 + nv;

            idx += 1;
        }
    }

    setCurrentElement("view_00");
    clearView();
    float u_offset = 0.0f, v_offset = 0.0f;

    for(size_t k = 0; k < num_its; k++)
    {
        evalKlein(u_mat, v_mat, x, y, z, u_offset, v_offset, klein_params);
        drawMesh(vx, vy, vz, indices.view(), properties::EdgeColor(0, 0, 0), properties::FaceColor(255, 0, 244));
        u_offset += 0.01;

        usleep(10000);

        softClearView();
    }
}

}

#endif // TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_PLOT2_H_
