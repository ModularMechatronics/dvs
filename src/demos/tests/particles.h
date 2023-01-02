#ifndef DEMOS_TESTS_PARTICLES_H_
#define DEMOS_TESTS_PARTICLES_H_

#include <Box2D/Box2D.h>
#include <Box2D/Particle/b2Particle.h>

#include <fstream>
#include <nlohmann/json.hpp>

#include "dvs/dvs.h"
#include "misc/color_map.h"
#include "rapidxml.hpp"

using namespace dvs;

namespace particles
{

template <typename T> struct ParametricLine2D
{
    // Point on line
    Point2<T> p;

    // Vector in line direction
    Vec2<T> v;

    ParametricLine2D(const Point2<T>& p_, const Vec2<T>& v_)
    {
        p = p_;
        v = v_;
    }
};

template <typename T> struct Line2D
{
    T a;
    T b;
    T c;

    Line2D(const T a_, const T b_, const T c_) : a{a_}, b{b_}, c{c_} {}

    Line2D(const ParametricLine2D<T>& pl)
    {
        a = -pl.v.y;
        b = pl.v.x;
        c = -(a * pl.p.x + b * pl.p.y);
    }

    Line2D() = default;

    T eval(const Point2<T>& p) const
    {
        return a * p.x + b * p.y + c;
    }

    bool isOnNormalVectorSide(const Point2<T>& p) const
    {
        // Returns true if p is on the same side as the side that the normal
        // vector of the line extends into (i.e. points towards)
        return this->eval(p) > 0;
    }
};

template <typename T> Line2D<T> homogeneousLineFromPoints(const Point2<T>& p0, const Point2<T>& p1)
{
    const Vec2<T> v = p1 - p0;
    const ParametricLine2D<T> line_p(p0, v);

    return Line2D<T>(line_p);
}

std::vector<size_t> findSubStringIndices(const std::string& str, const std::string& sub_str)
{
    std::vector<size_t> res_vec;

    if (str.size() > 0)
    {
        const char first_char = sub_str[0];
        if (sub_str.length() == 1)
        {
            for (size_t k = 0; k < str.length(); k++)
            {
                if (str[k] == first_char)
                {
                    res_vec.push_back(k);
                }
            }
        }
        else if (str.length() >= sub_str.length())
        {
            for (size_t k = 0; k < (str.length() + 1 - sub_str.length()); k++)
            {
                if (str[k] == first_char)
                {
                    const std::string current_sub_str = str.substr(k, sub_str.length());
                    if (current_sub_str == sub_str)
                    {
                        res_vec.push_back(k);
                    }
                }
            }
        }
    }
    return res_vec;
}

std::vector<std::string> splitString(const std::string& string_to_split, const std::string& delimiter)
{
    std::vector<std::string> split_string;

    const std::vector<size_t> indices = findSubStringIndices(string_to_split, delimiter);
    if (indices.size() == 0)
    {
        split_string.push_back(string_to_split);
    }
    else
    {
        split_string.resize(indices.size() + 1);
        if (indices[0] == 0)
        {
            split_string[0] = "";
        }
        else
        {
            split_string[0] = string_to_split.substr(0, indices[0]);
        }

        for (size_t k = 0; k < (indices.size() - 1); k++)
        {
            const size_t idx0 = indices[k] + delimiter.length();
            const size_t idx1 = indices[k + 1];

            split_string[k + 1] = string_to_split.substr(idx0, idx1 - idx0);
        }

        if (indices[indices.size() - 1] == (string_to_split.length() - 1))
        {
            split_string[indices.size()] = "";
        }
        else
        {
            split_string[indices.size()] =
                string_to_split.substr(indices[indices.size() - 1] + delimiter.length(),
                                       string_to_split.length() - indices[indices.size() - 1]);
        }
    }

    return split_string;
}

Vector<b2Vec2> splitPointsString(const std::string& s)
{
    const std::vector<std::string> coordinates_strings = splitString(s, " ");

    Vector<b2Vec2> coordinates{coordinates_strings.size() - 1};

    size_t idx = 0;
    for (size_t k = 0; k < coordinates_strings.size(); k++)
    {
        if (coordinates_strings[k] == "")
        {
            continue;
        }
        const std::vector<std::string> q = splitString(coordinates_strings[k], ",");

        coordinates(idx).x = (std::stof(q[0])) / 300.0f + 1.1f - 2.0f;
        coordinates(idx).y = (-std::stof(q[1])) / 300.0f + 4.0f;

        idx++;
    }

    return coordinates;
}

std::vector<Vector<b2Vec2>> readSVG()
{
    std::fstream newfile;
    newfile.open("../demos/tests/vectorpaint.svg", std::ios::in);

    std::string text_data;

    if (newfile.is_open())
    {
        std::string tp;
        while (getline(newfile, tp))
        {
            text_data.append(tp);
        }
        newfile.close();
    }

    rapidxml::xml_document<> doc;
    doc.parse<0>(text_data.data());

    float svg_width, svg_height;

    rapidxml::xml_node<>* node = doc.first_node("svg");

    for (rapidxml::xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute())
    {
        if (std::string(attr->name()) == "width")
        {
            const std::string ws = attr->value();
            svg_width = std::stof(ws);
        }
        else if (std::string(attr->name()) == "height")
        {
            const std::string hs = attr->value();
            const float svg_height = std::stof(hs);
        }
    }

    std::vector<Vector<b2Vec2>> boundaries;

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

void plotBoundaries(const Vector<b2Vec2>& vertices, const properties::Color& c)
{
    Vector<float> x{vertices.size()}, y{vertices.size()};

    for (size_t k = 0; k < vertices.size(); k++)
    {
        x(k) = vertices(k).x;
        y(k) = vertices(k).y;
    }

    plot(x, y, c);
}

class PointAssigner
{
private:
    std::vector<Vector<b2Vec2>> boundaries_;

    struct PointsAndBB
    {
        Vector<b2Vec2> points;
        Vector<Line2D<float>> lines;
        Vec2f min_vec;
        Vec2f max_vec;

        PointsAndBB() = default;
        PointsAndBB(const Vector<b2Vec2>& points_) : points{points_}, lines{points_.size()}
        {
            min_vec.x = points_(0).x;
            min_vec.y = points_(0).y;
            max_vec.x = points_(0).x;
            max_vec.y = points_(0).y;

            for (size_t k = 0; k < points_.size(); k++)
            {
                min_vec.x = std::min(points_(k).x, min_vec.x);
                min_vec.y = std::min(points_(k).y, min_vec.y);
                max_vec.x = std::max(points_(k).x, max_vec.x);
                max_vec.y = std::max(points_(k).y, max_vec.y);
            }

            for (size_t k = 0; k < points_.size() - 1U; k++)
            {
                const Point2f p0{points(k).x, points(k).y};
                const Point2f p1{points(k + 1U).x, points(k + 1U).y};
                lines(k) = homogeneousLineFromPoints(p0, p1);
            }

            const Point2f p0{points(points.size() - 1U).x, points(points.size() - 1U).y};
            const Point2f p1{points(0U).x, points(0U).y};
            lines(lines.size() - 1U) = homogeneousLineFromPoints(p0, p1);
        }

        bool pointIsIn(const Point2f& pt) const
        {
            for (size_t k = 0; k < lines.size(); k++)
            {
                if (!lines(k).isOnNormalVectorSide(pt))
                {
                    return false;
                }
            }

            return true;
        }
    };

    PointsAndBB p_outer_letter_;
    PointsAndBB p_inner_letter_;
    PointsAndBB l_letter_;
    PointsAndBB o0_outer_letter_;
    PointsAndBB o0_inner_letter_;

    Vector<RGB888> output_color_;

    void assignForLetter(const b2Vec2* const particles,
                         const size_t num_particles,
                         const PointsAndBB& pts_bb,
                         const RGB888& col)
    {
        const Vec2f min_vec = pts_bb.min_vec;
        const Vec2f max_vec = pts_bb.max_vec;
        for (size_t k = 0; k < num_particles; k++)
        {
            const Vec2f pt{particles[k].x, particles[k].y};

            if ((pt.x < min_vec.x) || (pt.x > max_vec.x) || (pt.y < min_vec.y) || (pt.y > max_vec.y))
            {
                continue;
            }
            else if (pts_bb.pointIsIn(pt))
            {
                output_color_(k) = col;
            }
        }
    }

    void assignForLetter(const b2Vec2* const particles,
                         const size_t num_particles,
                         const PointsAndBB& pts_bb_inner,
                         const PointsAndBB& pts_bb_outer,
                         const RGB888& col)
    {
        const Vec2f min_vec = pts_bb_outer.min_vec;
        const Vec2f max_vec = pts_bb_outer.max_vec;
        for (size_t k = 0; k < num_particles; k++)
        {
            const Vec2f pt{particles[k].x, particles[k].y};

            /*if ((pt.x < min_vec.x) || (pt.x > max_vec.x) || (pt.y < min_vec.y) || (pt.y > max_vec.y))
            {
                continue;
            }
            else */
            if (pts_bb_outer.pointIsIn(pt))
            {
                output_color_(k) = col;
            }
            // else if (pts_bb_inner.pointIsIn(pt) && (!pts_bb_outer.pointIsIn(pt)))
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

        p_outer_letter_ = PointsAndBB(boundaries_[0]);
        p_inner_letter_ = PointsAndBB(boundaries_[1]);
        l_letter_ = PointsAndBB(boundaries_[2]);
        o0_outer_letter_ = PointsAndBB(boundaries_[3]);
        o0_inner_letter_ = PointsAndBB(boundaries_[4]);
    }

    VectorConstView<RGB888> getColors() const
    {
        return output_color_.constView();
    }

    void assignColors(const b2Vec2* const particles, const size_t num_particles)
    {
        output_color_.resize(num_particles);

        for (size_t k = 0; k < num_particles; k++)
        {
            output_color_(k) = RGB888{0, 0, 0};
        }

        assignForLetter(particles, num_particles, p_inner_letter_, p_outer_letter_, RGB888{255, 0, 0});

        /*assignForLetter(particles, num_particles, l_letter_, RGB888{255, 0, 0});

        assignForLetter(particles, num_particles, o0_inner_letter_, o0_outer_letter_, RGB888{255, 255, 0});*/
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
    b2Body* static_body_;
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
    ParticleSystem(const size_t num_particles, const Vec2f min_bnd, const Vec2f max_bnd)
        : num_particles_{num_particles}, min_bnd_{min_bnd}, max_bnd_{max_bnd}, world_{b2Vec2{0.0, -9.82}}
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
        particleGroupDef.position.Set(0.0f, 5.0f);
        particleGroupDef.shape = &shape;

        ((b2ParticleSystem*)particle_system_)->CreateParticleGroup(particleGroupDef);

        std::cout << "Num particles: " << particle_system_->GetParticleCount() << std::endl;

        x_pos_ = new float[particle_system_->GetParticleCount()];
        y_pos_ = new float[particle_system_->GetParticleCount()];
        x_vel_ = new float[particle_system_->GetParticleCount()];
        y_vel_ = new float[particle_system_->GetParticleCount()];

        color_.resize(particle_system_->GetParticleCount());
    }

    ~ParticleSystem() {}

    size_t numPoints() const
    {
        return particle_system_->GetParticleCount();
    }

    const b2Vec2* getParticles() const
    {
        return particle_system_->GetPositionBuffer();
    }

    void update()
    {
        const b2Vec2* particles = particle_system_->GetPositionBuffer();
        const b2Vec2* velocities = particle_system_->GetVelocityBuffer();

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

        // scatter(x, y, color_.constView(), properties::ScatterStyle::Disc(), properties::PointSize(20));

        for (size_t k = 0; k < 1; k++)
        {
            world_.Step(0.01f, 8, 3);
        }
    }

    float* getXPos() const
    {
        return x_pos_;
    }

    float* getYPos() const
    {
        return y_pos_;
    }
};

void testBasic()
{
    const size_t num_steps = 1;
    const size_t num_particles = 500;
    const Vec2f min_bnd{-4.0f, -4.0f};
    const Vec2f max_bnd{4.0f, 4.0f};

    setCurrentElement("p_view_0");
    clearView();
    waitForFlush();
    axis(min_bnd, max_bnd);

    PointAssigner pa{};

    ParticleSystem ps{num_particles, min_bnd, max_bnd};

    VectorConstView<float> x{ps.getXPos(), ps.numPoints()};
    VectorConstView<float> y{ps.getYPos(), ps.numPoints()};

    pa.assignColors(ps.getParticles(), ps.numPoints());

    for (size_t k = 0; k < num_steps; k++)
    {
        ps.update();
        scatter(x, y, pa.getColors(), properties::ScatterStyle::Disc(), properties::PointSize(20));
        flushElement();
        usleep(50 * 1000);
        // softClearView();
    }
}

}  // namespace particles

#endif  // DEMOS_TESTS_PARTICLES_H_
