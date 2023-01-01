#ifndef DEMOS_TESTS_PARTICLES_H_
#define DEMOS_TESTS_PARTICLES_H_

#include <Box2D/Box2D.h>
#include <Box2D/Particle/b2Particle.h>

#include <fstream>

#include "dvs/dvs.h"
#include "misc/color_map.h"
#include "rapidxml.hpp"

using namespace dvs;

namespace particles
{

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

    Vector<b2Vec2> coordinates{coordinates_strings.size() - 1U};

    size_t idx = 0;
    for (size_t k = 0; k < coordinates_strings.size(); k++)
    {
        if (coordinates_strings[k] == "")
        {
            continue;
        }
        const std::vector<std::string> q = splitString(coordinates_strings[k], ",");

        coordinates(idx).x = (std::stof(q[0])) / 384.0f + 1.1f;
        coordinates(idx).y = (-std::stof(q[1])) / 384.0f;

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

void plotBoundaries(const Vector<b2Vec2>& vertices)
{
    Vector<float> x{vertices.size()}, y{vertices.size()};

    for (size_t k = 0; k < vertices.size(); k++)
    {
        x(k) = vertices(k).x;
        y(k) = vertices(k).y;
    }

    plot(x, y);
}

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
    std::vector<Vector<b2Vec2>> boundaries_;

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
        b2Body* ground = world_.CreateBody(&bd);

        boundaries_ = readSVG();

        {
            b2PolygonShape shape;
            const Vector<b2Vec2> vertices{VectorInitializer{b2Vec2(-4, -2), b2Vec2(4, -2), b2Vec2(5, -2)}};
            shape.Set(vertices.data(), vertices.size());
            ground->CreateFixture(&shape, 0.0f);
        }

        for (size_t k = 0; k < boundaries_.size(); k++)
        {
            b2PolygonShape shape;
            shape.Set(boundaries_[k].data(), boundaries_[k].size());
            ground->CreateFixture(&shape, 1.0f);
        }

        b2PolygonShape shape;
        shape.SetAsBox(1.5f, 1.5f);

        b2ParticleGroupDef particleGroupDef;
        particleGroupDef.flags = b2_waterParticle;
        particleGroupDef.position.Set(0.0f, 2.0f);
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

        scatter(x, y, color_.constView(), properties::ScatterStyle::Disc(), properties::PointSize(20));

        for (size_t k = 0; k < boundaries_.size(); k++)
        {
            plotBoundaries(boundaries_[k]);
        }
        world_.Step(0.01f, 8, 3);
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
    const size_t num_steps = 500;
    const size_t num_particles = 500;
    const Vec2f min_bnd{-4.0f, -4.0f};
    const Vec2f max_bnd{4.0f, 4.0f};

    setCurrentElement("p_view_0");
    clearView();
    waitForFlush();
    axis(min_bnd, max_bnd);

    ParticleSystem ps{num_particles, min_bnd, max_bnd};

    for (size_t k = 0; k < num_steps; k++)
    {
        ps.update();
        flushElement();
        usleep(50 * 1000);
        softClearView();
    }
}

}  // namespace particles

#endif  // DEMOS_TESTS_PARTICLES_H_
