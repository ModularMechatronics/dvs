#ifndef DEMOS_TESTS_PARTICLES_H_
#define DEMOS_TESTS_PARTICLES_H_

#include <Box2D/Box2D.h>
#include <Box2D/Particle/b2Particle.h>

#include <fstream>
#include <nlohmann/json.hpp>

#include "dvs/dvs.h"
#include "helper_functions.h"
#include "line2d.h"
#include "misc/color_map.h"
#include "polygon.h"
#include "rapidxml.hpp"

using namespace dvs;

namespace particles
{

Vector<Point2d> splitPointsString(
    const std::string& s, const double mul, const double add_x, const double add_y, const double sign_mul)
{
    const std::vector<std::string> coordinates_strings = splitString(s, " ");

    std::vector<Point2d> coords;
    coords.reserve(coordinates_strings.size() - 1);
    coords.push_back(Point2d());

    const std::vector<std::string> q = splitString(coordinates_strings[0], ",");

    coords[0].x = (std::stof(q[0])) * mul + add_x;
    coords[0].y = (-std::stof(q[1])) * mul + add_y;

    size_t idx = 1;
    for (size_t k = 0; k < coordinates_strings.size(); k++)
    {
        if (coordinates_strings[k] == "")
        {
            continue;
        }
        const std::vector<std::string> q = splitString(coordinates_strings[k], ",");

        const double x = (std::stod(q[0])) * mul + add_x;
        const double y = (sign_mul * std::stod(q[1])) * mul + add_y;

        if ((x == coords[idx - 1U].x) && (y == coords[idx - 1U].y))
        {
            continue;
        }

        coords.push_back(Point2d{x, y});

        idx++;
    }

    Vector<Point2d> coordinates{coords};

    return coordinates;
}

Vector<Point2d> splitPointsString(const std::string& s)
{
    const double mul = 1.0 / 130.0;
    const double add_x = -2.6;
    const double add_y = 0.5;

    return splitPointsString(s, mul, add_x, add_y, -1.0);
}

std::string readTextFile(const std::string& file_path)
{
    std::fstream txt_file;
    txt_file.open(file_path, std::ios::in);

    std::string text_data;

    if (txt_file.is_open())
    {
        std::string tp;
        while (getline(txt_file, tp))
        {
            text_data.append(tp);
        }
        txt_file.close();
    }

    return text_data;
}

std::vector<Vector<Point2d>> readSVG()
{
    std::string text_data = readTextFile("../demos/tests/vectorpaint.svg");

    rapidxml::xml_document<> doc;
    doc.parse<0>(text_data.data());

    // float svg_width, svg_height;

    rapidxml::xml_node<>* node = doc.first_node("svg");

    std::vector<Vector<Point2d>> boundaries;

    for (rapidxml::xml_node<>* child = node->first_node(); child; child = child->next_sibling())
    {
        const std::string child_name = child->name();
        if (child_name == "g")
        {
            for (rapidxml::xml_node<>* sub_child = child->first_node(); sub_child;
                 sub_child = sub_child->next_sibling())
            {
                for (rapidxml::xml_attribute<>* attr = sub_child->first_attribute(); attr;
                     attr = attr->next_attribute())
                {
                    const std::string attr_name = attr->name();
                    if (attr_name == "points")
                    {
                        boundaries.push_back(splitPointsString(attr->value()));
                    }
                }
            }
        }
    }

    return boundaries;
}

void plotBoundaries(const Vector<Point2d>& vertices, const properties::Color& c)
{
    Vector<double> x{vertices.size()}, y{vertices.size()};

    for (size_t k = 0; k < vertices.size(); k++)
    {
        x(k) = vertices(k).x;
        y(k) = vertices(k).y;
    }

    plot(x, y, c);
}

void scatterBoundaries(const Vector<Point2d>& vertices, const properties::Color& c)
{
    Vector<double> x{vertices.size()}, y{vertices.size()};

    for (size_t k = 0; k < vertices.size(); k++)
    {
        x(k) = vertices(k).x;
        y(k) = vertices(k).y;
    }

    scatter(x, y, c);
}

class PointAssigner
{
private:
    std::vector<Vector<Point2d>> boundaries_;

    Polygon p_outer_letter_;
    Polygon p_inner_letter_;
    Polygon l_letter_;
    Polygon o_outer_letter_;
    Polygon o_inner_letter_;

    Vector<RGB888> output_color_;

    void assignForLetter(const Vector<Point2d>& points, const Polygon& polygon, const RGB888& col)
    {
        const Vec2d min_vec = polygon.getMinVec();
        const Vec2d max_vec = polygon.getMaxVec();

        for (size_t k = 0; k < points.size(); k++)
        {
            const Vec2d pt{points(k).x, points(k).y};

            if ((pt.x < min_vec.x) || (pt.x > max_vec.x) || (pt.y < min_vec.y) || (pt.y > max_vec.y))
            {
                continue;
            }
            else if (polygon.pointIsInPolygon(pt))
            {
                output_color_(k) = col;
            }
        }
    }

public:
    PointAssigner()
    {
        boundaries_ = readSVG();

        plotBoundaries(boundaries_[0], properties::Color::Red());
        plotBoundaries(boundaries_[1], properties::Color::Green());
        plotBoundaries(boundaries_[2], properties::Color::Blue());
        plotBoundaries(boundaries_[3], properties::Color::Black());
        plotBoundaries(boundaries_[4], properties::Color::Magenta());

        p_outer_letter_ = Polygon(boundaries_[0]);
        p_inner_letter_ = Polygon(boundaries_[1]);
        l_letter_ = Polygon(boundaries_[2]);
        o_outer_letter_ = Polygon(boundaries_[3]);
        o_inner_letter_ = Polygon(boundaries_[4]);
    }

    VectorConstView<RGB888> getColors() const
    {
        return output_color_.constView();
    }

    void assignColors(const Vector<Point2d>& points)
    {
        output_color_.resize(points.size());

        for (size_t k = 0; k < points.size(); k++)
        {
            output_color_(k) = RGB888{0, 0, 0};
        }

        assignForLetter(points, p_outer_letter_, RGB888{255, 0, 0});
        assignForLetter(points, p_inner_letter_, RGB888{0, 0, 0});
        assignForLetter(points, l_letter_, RGB888{0, 255, 0});
        assignForLetter(points, o_outer_letter_, RGB888{0, 0, 255});
        assignForLetter(points, o_inner_letter_, RGB888{0, 0, 0});
    }
};

class ParticleSystem
{
private:
    size_t num_particles_;
    Vec2f min_bnd_;
    Vec2f max_bnd_;
    float* x_pos_;
    float* y_pos_;

    float* x_vel_;
    float* y_vel_;

    b2World world_;
    b2ParticleSystem* particle_system_;
    Vector<RGB888> color_;

    Vector<b2Vec2> vertices_;
    b2Body* ground_;

    void placeTipAt(const float x, const float y)
    {
        const float height = 1.2f;
        const float width = 1.2f;
        const float height2 = height2 / 2.0f;
        const float width2 = width2 / 2.0f;

        b2PolygonShape shape;
        const Vector<b2Vec2> vertices{
            VectorInitializer{b2Vec2(x + -0.1, y + -0.2), b2Vec2(x + 0.0, y + 0.2), b2Vec2(x + 0.1, y + -0.2)}};
        shape.Set(vertices.data(), vertices.size());
        ground_->CreateFixture(&shape, 0.0f);
    }

public:
    ParticleSystem() = delete;
    ParticleSystem(const Vec2f min_bnd, const Vec2f max_bnd)
        : min_bnd_{min_bnd}, max_bnd_{max_bnd}, world_{b2Vec2{0.0, -9.82}}
    {
        b2ParticleSystemDef particleSystemDef;

        particleSystemDef.radius = 0.035f;
        particleSystemDef.dampingStrength = 0.2;
        particleSystemDef.gravityScale = 1.0;
        particleSystemDef.density = 1.0;

        particle_system_ = world_.CreateParticleSystem(&particleSystemDef);

        b2BodyDef bd;
        ground_ = world_.CreateBody(&bd);

        placeTipAt(0.0f, 1.6f);
        placeTipAt(-0.4f, 1.6f);
        placeTipAt(0.4f, 1.6f);
        placeTipAt(-0.8f, 1.6f);
        placeTipAt(0.8f, 1.6f);
        placeTipAt(-1.2f, 1.6f);
        placeTipAt(1.2f, 1.6f);
        placeTipAt(-1.6f, 1.6f);
        placeTipAt(1.6f, 1.6f);

        placeTipAt(-0.2f, 1.0f);
        placeTipAt(0.2f, 1.0f);
        placeTipAt(-0.6f, 1.0f);
        placeTipAt(0.6f, 1.0f);
        placeTipAt(-1.0f, 1.0f);
        placeTipAt(1.0f, 1.0f);

        {
            b2PolygonShape shape;
            const Vector<b2Vec2> vertices{VectorInitializer{b2Vec2(-4, -2), b2Vec2(4, -2), b2Vec2(-4, -4)}};
            shape.Set(vertices.data(), vertices.size());
            ground_->CreateFixture(&shape, 0.0f);
        }

        {
            b2PolygonShape shape;
            const Vector<b2Vec2> vertices{VectorInitializer{b2Vec2(-4, 4), b2Vec2(-2, 2), b2Vec2(-2, -2)}};
            shape.Set(vertices.data(), vertices.size());
            ground_->CreateFixture(&shape, 0.0f);
        }

        {
            b2PolygonShape shape;
            const Vector<b2Vec2> vertices{VectorInitializer{b2Vec2(4, 4), b2Vec2(2, 2), b2Vec2(2, -2)}};
            shape.Set(vertices.data(), vertices.size());
            ground_->CreateFixture(&shape, 0.0f);
        }

        b2PolygonShape shape;
        shape.SetAsBox(2.0f, 2.0f);

        b2ParticleGroupDef particleGroupDef;
        particleGroupDef.flags = b2_waterParticle;
        particleGroupDef.position.Set(0.0f, 4.1f);
        particleGroupDef.shape = &shape;

        particle_system_->CreateParticleGroup(particleGroupDef);

        num_particles_ = particle_system_->GetParticleCount();

        x_pos_ = new float[particle_system_->GetParticleCount()];
        y_pos_ = new float[particle_system_->GetParticleCount()];
        x_vel_ = new float[particle_system_->GetParticleCount()];
        y_vel_ = new float[particle_system_->GetParticleCount()];

        color_.resize(particle_system_->GetParticleCount());
    }

    ~ParticleSystem() {}

    size_t numPoints() const
    {
        return num_particles_;
    }

    const b2Vec2* getParticles() const
    {
        return particle_system_->GetPositionBuffer();
    }

    void update()
    {
        const b2Vec2* const particles = particle_system_->GetPositionBuffer();
        const b2Vec2* const velocities = particle_system_->GetVelocityBuffer();

        const size_t num_particles = particle_system_->GetParticleCount();

        for (size_t k = 0; k < num_particles; k++)
        {
            x_pos_[k] = particles[k].x;
            y_pos_[k] = particles[k].y;

            const float vx = velocities[k].x;
            const float vy = velocities[k].y;

            const float v = std::sqrt(vx * vx + vy * vy);
            const float theta = std::atan2(vy, vx) / (M_PI * 2.0) + 0.5f;

            auto const c = color_maps::jet(theta);

            color_(k) = RGB888{c.red, c.green, c.blue};
        }

        VectorConstView<float> x{x_pos_, num_particles};
        VectorConstView<float> y{y_pos_, num_particles};

        auto const cm = color_maps::viridis;

        for (size_t k = 0; k < 1; k++)
        {
            world_.Step(0.01f, 8, 3);
        }
    }

    VectorConstView<float> getXView() const
    {
        return VectorConstView<float>{x_pos_, num_particles_};
    }

    VectorConstView<float> getYView() const
    {
        return VectorConstView<float>{y_pos_, num_particles_};
    }

    VectorConstView<RGB888> getColorView() const
    {
        return color_.constView();
    }

    void savePointsToFile() const
    {
        const std::string file_path = "../demos/tests/points.txt";

        std::ofstream myfile;
        myfile.open(file_path);

        const VectorConstView<float> x{x_pos_, num_particles_};
        const VectorConstView<float> y{y_pos_, num_particles_};

        for (size_t k = 0; k < num_particles_; k++)
        {
            const std::string sx = std::to_string(x(k));
            const std::string sy = std::to_string(y(k));

            myfile << sx << "," << sy << " ";
        }

        myfile.close();
    }
};

Vector<Point2d> readSavedFile()
{
    const std::string text_data = readTextFile("../demos/tests/points.txt");

    return splitPointsString(text_data, 1.0, 0.0, 0.0, 1.0);
}

void testBasicShow()
{
    const size_t num_steps = 700;
    const Vec2f min_bnd{-4.0f, 0.0f};
    const Vec2f max_bnd{4.0f, 8.0f};

    setCurrentElement("p_view_0");
    clearView();
    waitForFlush();
    axis({-3.0, -2.1}, {3.0, 1.0});

    const Vector<Point2d> saved_points = readSavedFile();

    PointAssigner pa{};
    ParticleSystem ps{min_bnd, max_bnd};

    // const VectorConstView<float> x = ps.getXView();
    // const VectorConstView<float> y = ps.getYView();

    pa.assignColors(saved_points);

    // scatterBoundaries(saved_points, properties::Color(255, 0, 0));

    Vector<double> x{saved_points.size()}, y{saved_points.size()};

    for (size_t k = 0; k < saved_points.size(); k++)
    {
        x(k) = saved_points(k).x;
        y(k) = saved_points(k).y;
    }

    scatter(x.constView(), y.constView(), pa.getColors());

    /*for (size_t k = 0; k < num_steps; k++)
    {
        ps.update();
        scatter(x, y, pa.getColors(), properties::ScatterStyle::Disc(), properties::PointSize(20));
        flushElement();
        usleep(50 * 1000);
        softClearView();
    }*/
}

void testBasic()
{
    const size_t num_steps = 700;
    const Vec2f min_bnd{-4.0f, 0.0f};
    const Vec2f max_bnd{4.0f, 8.0f};

    setCurrentElement("p_view_0");
    clearView();
    waitForFlush();
    axis({-3.0, -2.1}, {3.0, 1.0});

    ParticleSystem ps{min_bnd, max_bnd};

    const VectorConstView<float> x = ps.getXView();
    const VectorConstView<float> y = ps.getYView();

    for (size_t k = 0; k < num_steps; k++)
    {
        ps.update();
        scatter(x, y, properties::Color::Red(), properties::ScatterStyle::Disc(), properties::PointSize(20));
        flushElement();
        usleep(50 * 1000);
        softClearView();
    }

    ps.savePointsToFile();
}

void testBasicTmp()
{
    const size_t num_polygon_points = 4;
    const size_t num_points = 1000;
    double t = 0.0;
    Vector<Point2d> polygon_points{num_polygon_points};
    Vector<double> xr{num_points}, yr{num_points};
    Vector<RGB888> colors{num_points};

    const auto r_fun = []() -> double {
        const double r = static_cast<double>(rand() % 1001) / 500.0 - 1.0;
        return r;
    };

    for (size_t k = 0; k < num_points; k++)
    {
        xr(k) = r_fun() * 1.4;
        yr(k) = r_fun() * 1.4;
    }

    const std::vector<Vector<Point2d>> boundaries = readSVG();

    polygon_points = boundaries[0];

    Polygon polygon{polygon_points};

    for (size_t k = 0; k < num_points; k++)
    {
        if (polygon.pointIsInPolygon({xr(k), yr(k)}))
        {
            colors(k) = RGB888(255, 0, 0);
        }
        else
        {
            colors(k) = RGB888(0, 0, 0);
        }
    }

    setCurrentElement("p_view_0");
    clearView();
    axis({-1.0, -1.0}, {1.0, 1.0});

    polygon.visualize();
    scatter(xr, yr, colors, properties::PointSize(20));
}

}  // namespace particles

#endif  // DEMOS_TESTS_PARTICLES_H_
