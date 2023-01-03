#ifndef DEMOS_TESTS_PARTICLES_H_
#define DEMOS_TESTS_PARTICLES_H_

#include <Box2D/Box2D.h>
#include <Box2D/Particle/b2Particle.h>

#include <fstream>
#include <nlohmann/json.hpp>

#include "dvs/dvs.h"
#include "line2d.h"
#include "misc/color_map.h"
#include "rapidxml.hpp"

using namespace dvs;

namespace particles
{

class Polygon
{
private:
    struct PointPair
    {
        Point2d p0;
        Point2d p1;
    };

    Vector<PointPair> point_pairs_;
    Vec2d min_vec_;
    Vec2d max_vec_;
    Vec2d random_vec_;
    Vec2d perp_random_vec_;

    enum class IntersectionType
    {
        Intersects,
        DoesNotIntersect,
        Collinear
    };

public:
    Polygon() {}

    Polygon(const Vector<Point2d>& points)
    {
        point_pairs_.resize(points.size());

        min_vec_ = points(0);
        max_vec_ = points(0);

        for (size_t k = 0; k < points.size(); k++)
        {
            min_vec_.x = std::min(min_vec_.x, points(k).x);
            min_vec_.y = std::min(min_vec_.y, points(k).y);

            max_vec_.x = std::max(max_vec_.x, points(k).x);
            max_vec_.y = std::max(max_vec_.y, points(k).y);
        }

        random_vec_ = Vec2d{std::sqrt(2.0), std::sqrt(7.0)};
        perp_random_vec_ = Vec2d{-random_vec_.y, random_vec_.x};

        for (size_t k = 0; k < (points.size() - 1U); k++)
        {
            point_pairs_(k).p0 = points(k);
            point_pairs_(k).p1 = points(k + 1);
        }

        point_pairs_(point_pairs_.size() - 1U).p0 = points(points.size() - 1U);
        point_pairs_(point_pairs_.size() - 1U).p1 = points(0U);
    }

    void visualize() const
    {
        Vector<double> x{point_pairs_.size() + 1}, y{point_pairs_.size() + 1};

        for (size_t k = 0; k < point_pairs_.size(); k++)
        {
            x(k) = point_pairs_(k).p0.x;
            y(k) = point_pairs_(k).p0.y;
        }
        x(x.size() - 1U) = point_pairs_(0).p0.x;
        y(y.size() - 1U) = point_pairs_(0).p0.y;

        scatter(x, y);
        plot(x, y);
    }

    bool pointIsInPolygon(const Point2d& pt) const
    {
        if ((pt.x < min_vec_.x) || (pt.x > max_vec_.x) || (pt.y < min_vec_.y) || (pt.y > max_vec_.x))
        {
            return false;
        }

        int num_crossings = 0;

        const Point2d q0 = pt;
        const Point2d q1 = pt + random_vec_;

        for (size_t k = 0; k < point_pairs_.size(); k++)
        {
            const Point2d p0 = point_pairs_(k).p0;
            const Point2d p1 = point_pairs_(k).p1;

            /*const double x1 = q0.x;
            const double y1 = q0.y;
            const double x2 = q1.x;
            const double y2 = q1.y;

            const double x3 = p0.x;
            const double y3 = p0.y;
            const double x4 = p1.x;
            const double y4 = p1.y;

            const double t =
                ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
            if ((t >= 0.0) && (t <= 1.0))
            {
                num_crossings++;
            }*/

            const Point2d q0 = pt;
            const Point2d q1 = pt + random_vec_;

            IntersectionType intersection_type = areIntersecting(p0.x, p0.y, p1.x, p1.y, q0.x, q0.y, q1.x, q1.y);

            if (intersection_type == IntersectionType::Collinear)
            {
                const Point2d q0 = pt;
                const Point2d q1 = pt + perp_random_vec_;

                intersection_type = areIntersecting(p0.x, p0.y, p1.x, p1.y, q0.x, q0.y, q1.x, q1.y);
            }

            if (intersection_type == IntersectionType::Intersects)
            {
                num_crossings++;
            }
        }

        return (num_crossings % 2) == 1;
    }

    IntersectionType areIntersecting(const double v1x1,
                                     const double v1y1,
                                     const double v1x2,
                                     const double v1y2,
                                     const double v2x1,
                                     const double v2y1,
                                     const double v2x2,
                                     const double v2y2) const
    {
        double d1, d2;
        double a1, a2, b1, b2, c1, c2;

        // Convert vector 1 to a line (line 1) of infinite length.
        // We want the line in linear equation standard form: A*x + B*y + C = 0
        // See: http://en.wikipedia.org/wiki/Linear_equation
        a1 = v1y2 - v1y1;
        b1 = v1x1 - v1x2;
        c1 = (v1x2 * v1y1) - (v1x1 * v1y2);

        // Every point (x,y), that solves the equation above, is on the line,
        // every point that does not solve it, is not. The equation will have a
        // positive result if it is on one side of the line and a negative one
        // if is on the other side of it. We insert (x1,y1) and (x2,y2) of vector
        // 2 into the equation above.
        d1 = (a1 * v2x1) + (b1 * v2y1) + c1;
        d2 = (a1 * v2x2) + (b1 * v2y2) + c1;

        // If d1 and d2 both have the same sign, they are both on the same side
        // of our line 1 and in that case no intersection is possible. Careful,
        // 0 is a special case, that's why we don't test ">=" and "<=",
        // but "<" and ">".
        if ((d1 > 0.0) && (d2 > 0.0))
        {
            return IntersectionType::DoesNotIntersect;
        }
        if ((d1 < 0.0) && (d2 < 0.0))
        {
            return IntersectionType::DoesNotIntersect;
        }

        // The fact that vector 2 intersected the infinite line 1 above doesn't
        // mean it also intersects the vector 1. Vector 1 is only a subset of that
        // infinite line 1, so it may have intersected that line before the vector
        // started or after it ended. To know for sure, we have to repeat the
        // the same test the other way round. We start by calculating the
        // infinite line 2 in linear equation standard form.
        a2 = v2y2 - v2y1;
        b2 = v2x1 - v2x2;
        c2 = (v2x2 * v2y1) - (v2x1 * v2y2);

        // Calculate d1 and d2 again, this time using points of vector 1.
        d1 = (a2 * v1x1) + (b2 * v1y1) + c2;
        d2 = (a2 * v1x2) + (b2 * v1y2) + c2;

        // Again, if both have the same sign (and neither one is 0),
        // no intersection is possible.
        if ((d1 > 0) && (d2 > 0))
        {
            return IntersectionType::DoesNotIntersect;
        }
        if ((d1 < 0) && (d2 < 0))
        {
            return IntersectionType::DoesNotIntersect;
        }

        // If we get here, only two possibilities are left. Either the two
        // vectors intersect in exactly one point or they are collinear, which
        // means they intersect in any number of points from zero to infinite.
        if (std::fabs((a1 * b2) - (a2 * b1)) < 1e-7)
        {
            return IntersectionType::Collinear;
        }

        // If they are not collinear, they must intersect in exactly one point.
        return IntersectionType::Intersects;
    }
};

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

    // Vector<b2Vec2> coordinates{coordinates_strings.size() - 1};
    std::vector<b2Vec2> coords;
    coords.reserve(coordinates_strings.size() - 1);
    coords.push_back(b2Vec2());

    const std::vector<std::string> q = splitString(coordinates_strings[0], ",");

    const float mul = 1.0f / 200.0f;
    const float add_x = -0.9f;
    const float add_y = 6.0f;

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

        const float x = (std::stof(q[0])) * mul + add_x;
        const float y = (-std::stof(q[1])) * mul + add_y;

        if ((x == coords[idx - 1U].x) && (y == coords[idx - 1U].y))
        {
            continue;
        }

        coords.push_back(b2Vec2(x, y));

        idx++;
    }

    Vector<b2Vec2> coordinates{coords};

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
        Vector<ParametricLine2D<double>> lines;
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
                const Point2d p0{points(k).x, points(k).y};
                const Point2d p1{points(k + 1U).x, points(k + 1U).y};
                lines(k) = parametricLineFromPoints(p0, p1);
            }

            const Point2d p0{points(points.size() - 1U).x, points(points.size() - 1U).y};
            const Point2d p1{points(0U).x, points(0U).y};
            lines(lines.size() - 1U) = parametricLineFromPoints(p0, p1);
        }

        bool pointIsIn(const Point2d& pt) const
        {
            const Point2d& random_point{10.0, 10.0};
            const ParametricLine2D<double> candidate_line = parametricLineFromPoints(pt, random_point);
            int num_crossings = 0;

            for (size_t k = 0; k < lines.size(); k++)
            {
                const HomogeneousLine2D<double> hom_candidate = candidate_line;
                const HomogeneousLine2D<double> home_line_k = lines(k);
                const Point2d intersection_point = hom_candidate.lineIntersection(home_line_k);

                const double tx = lines(k).tFromX(intersection_point.x);
                const double ty = lines(k).tFromY(intersection_point.y);

                double t = tx;

                if (isnan(t))
                {
                    t = ty;
                }

                if (isnan(t))
                {
                    int a = 0;
                }
                std::cout << tx << ", " << ty << std::endl;

                if ((t >= -1.0) && (t <= 1.0))
                {
                    num_crossings++;
                }
            }

            return (num_crossings % 2) == 1;
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
            const Vec2d pt{particles[k].x, particles[k].y};

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

        assignForLetter(particles, num_particles, p_outer_letter_, RGB888{255, 0, 0});
        // assignForLetter(particles, num_particles, p_inner_letter_, RGB888{0, 0, 0});
        // assignForLetter(particles, num_particles, p_inner_letter_, p_outer_letter_, RGB888{255, 0, 0});

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

void testBasicTmp()
{
    const size_t num_steps = 1;
    const size_t num_particles = 500;
    const Vec2f min_bnd{-4.0f, 0.0f};
    const Vec2f max_bnd{4.0f, 8.0f};

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

void testBasic()
{
    const size_t num_polygon_points = 10;
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

    for (size_t k = 0; k < num_polygon_points; k++)
    {
        polygon_points(k).x = std::cos(t);
        polygon_points(k).y = std::sin(t);
        t += 2.0 * M_PI / static_cast<double>(num_polygon_points);
    }
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
