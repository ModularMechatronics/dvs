#include "particles_density.h"
#include "tests/slam/color_maps.h"

namespace particles_density
{


constexpr size_t num_steps = 3200;
const Vec2f min_bnd{-4.0f, 0.0f};
const Vec2f max_bnd{4.0f, 26.0f};
constexpr float radius = 0.025f;
float start_y = -10.0f;

struct ParticleEjector
{
    b2ParticleGroup* group_;
    b2ParticleSystem* particle_system_;
    size_t num_particles_left_;
    size_t idx_;
    size_t offset_;
    Point2f eject_pos_;
    Vec2f eject_dir_;
    float angle_;
    float cos_angle_;
    float sin_angle_;
    b2Vec2* particles_;
    b2Vec2* velocities_;

    ParticleEjector() : group_{nullptr} {}
    ParticleEjector(b2ParticleGroup* group, b2ParticleSystem* particle_system, const Point2f& eject_pos, const Vec2f& eject_dir) : 
        group_{group},
        particle_system_{particle_system},
        eject_pos_{eject_pos},
        eject_dir_{eject_dir}
    {
        particles_ = particle_system_->GetPositionBuffer();
        velocities_ = particle_system_->GetVelocityBuffer();

        num_particles_left_ = group_->GetParticleCount();
        idx_ = 0U;
        offset_ = group_->GetBufferIndex();
        angle_ = -std::atan2(eject_dir_.y, eject_dir_.x);
        cos_angle_ = std::cos(angle_);
        sin_angle_ = std::sin(angle_);

        for(size_t k = 0; k < num_particles_left_; k++)
        {
            particles_[offset_ + k].x = k * 10.0f;
            particles_[offset_ + k].y = start_y;

            velocities_[offset_ + k].x = 0.0f;
            velocities_[offset_ + k].y = 0.0f;
        }

        start_y = start_y - 5.0f;

    }

    void eject()
    {
        if(num_particles_left_ > 0)
        {
            constexpr float delta = 0.05;
            constexpr size_t num_particles_to_emit = 5U;
            constexpr float full_width = static_cast<float>(num_particles_to_emit - 1U) * delta;
            float d = -full_width / 2.0f;

            for(size_t k = 0; k < num_particles_to_emit && num_particles_left_ > 0; k++)
            {
                const float y = d;
                d += delta;

                const float xr = sin_angle_ * y;
                const float yr = cos_angle_ * y;

                particles_[offset_ + idx_].x = eject_pos_.x + xr;
                particles_[offset_ + idx_].y = eject_pos_.y + yr;

                velocities_[offset_ + idx_].x = eject_dir_.x;
                velocities_[offset_ + idx_].y = eject_dir_.y;

                idx_++;
                num_particles_left_--;

            }
        }
    }
};

void ParticleSystem::changeDensity()
{
    auto& heaviest_grp = groups_.back();

    const size_t offset = heaviest_grp->GetBufferIndex();
    const size_t num_particles = heaviest_grp->GetParticleCount();

    float* densities = particle_system_->GetDensityBuffer();
    for(size_t k = 0; k < num_particles; k++)
    {
        densities[k + offset] = 0.0005;
    }

}

ParticleSystem::ParticleSystem(const Vec2f min_bnd, const Vec2f max_bnd)
    : min_bnd_{min_bnd}, max_bnd_{max_bnd}, world_{b2Vec2{0.0, -9.82}}
{

    b2ParticleSystemDef particleSystemDef;

    particleSystemDef.radius = radius;
    particleSystemDef.dampingStrength = 0.01;
    particleSystemDef.gravityScale = 0.15;
    particleSystemDef.density = 0.001;

    particle_system_ = world_.CreateParticleSystem(&particleSystemDef);

    b2BodyDef bd;
    ground_ = world_.CreateBody(&bd);

    {
        b2PolygonShape shape;
        const Vector<b2Vec2> vertices{VectorInitializer{b2Vec2(-4, -2), b2Vec2(4, -2), b2Vec2(-4, -4)}};
        shape.Set(vertices.data(), vertices.size());
        ground_->CreateFixture(&shape, 0.0f);
    }

    {
        b2PolygonShape shape;
        const Vector<b2Vec2> vertices{VectorInitializer{b2Vec2(-4, 18), b2Vec2(-2, 2), b2Vec2(-2, -2)}};
        shape.Set(vertices.data(), vertices.size());
        ground_->CreateFixture(&shape, 0.0f);
    }

    {
        b2PolygonShape shape;
        const Vector<b2Vec2> vertices{VectorInitializer{b2Vec2(4, 18), b2Vec2(2, 2), b2Vec2(2, -2)}};
        shape.Set(vertices.data(), vertices.size());
        ground_->CreateFixture(&shape, 0.0f);
    }

    {
        b2PolygonShape shape;
        const Vector<b2Vec2> vertices{VectorInitializer{b2Vec2(-2, 4.85), b2Vec2(2, 4.85), b2Vec2(0, 10)}};
        shape.Set(vertices.data(), vertices.size());
        ground_->CreateFixture(&shape, 0.0f);
    }

    b2PolygonShape shape;
    shape.SetAsBox(0.7f, 0.7f);

    b2ParticleGroupDef particleGroupDef;
    particleGroupDef.flags = b2_waterParticle;
    particleGroupDef.position.Set(-1.0f, 4.0f);
    particleGroupDef.shape = &shape;
    particleGroupDef.strength = 0.1;

    // std::vector<float> densities{0.001, 0.003, 0.005, 0.007, 0.09, 0.01, 0.05};
    std::vector<float> densities{0.001, 0.005, 0.01, 0.05, 0.09, 0.12, 0.15};

    float y_pos = 3.0;
    for(float density : densities)
    {
        particleGroupDef.density = density;
        particleGroupDef.position.Set(1.0f, y_pos);
        groups_.push_back(particle_system_->CreateParticleGroup(particleGroupDef));
        y_pos += 1.5f;
    }

    num_particles_ = particle_system_->GetParticleCount();

    for(auto& grp : groups_)
    {
        const float px = (static_cast<float>(rand() % 1001U) / 1000.0f - 0.5f) * 2.0f;
        const float py = (static_cast<float>(rand() % 1001U) / 1000.0f - 0.5f) * 2.0f + 1.0;

        const float theta = -2.0f * M_PI * static_cast<float>(rand() % 1001U) / 1000.0f;

        const float vx = std::cos(theta) * 5.0f;
        const float vy = std::sin(theta) * 5.0f;
        ejectors_.push_back(ParticleEjector{grp, particle_system_, Point2f{px, py}, Vec2f{vx, vy}});
    }


    x_pos_ = new float[particle_system_->GetParticleCount()];
    y_pos_ = new float[particle_system_->GetParticleCount()];
}

ParticleSystem::~ParticleSystem() {}

size_t ParticleSystem::numPoints() const
{
    return num_particles_;
}

const b2Vec2* ParticleSystem::getParticles() const
{
    return particle_system_->GetPositionBuffer();
}

void ParticleSystem::update(const double dt)
{
    
    for(auto& ej : ejectors_)
    {
        ej.eject();
    }
    const b2Vec2* const particles = particle_system_->GetPositionBuffer();

    const size_t num_particles = particle_system_->GetParticleCount();

    for (size_t k = 0; k < num_particles; k++)
    {
        x_pos_[k] = particles[k].x;
        y_pos_[k] = particles[k].y;
    }

    world_.Step(dt, 8, 3);
}

VectorConstView<float> ParticleSystem::getXView() const
{
    return VectorConstView<float>{x_pos_, num_particles_};
}

VectorConstView<float> ParticleSystem::getYView() const
{
    return VectorConstView<float>{y_pos_, num_particles_};
}

properties::Color toColor(const RGBTripletf& col)
{
    return properties::Color{
        static_cast<std::uint8_t>(col.red * 255.0f),
        static_cast<std::uint8_t>(col.green * 255.0f),
        static_cast<std::uint8_t>(col.blue * 255.0f)};
}

void testBasic()
{
    const std::string project_file_path = "../../project_files/small_demo.duoplot";

    openProjectFile(project_file_path);

    // 456, 781

    setCurrentElement("p_view_0");
    clearView();
    waitForFlush();
    setAxesBoxScaleFactor({1.0, 1.0, 1.0});
    // axis({-2.0, -1.712719}, {2.0, 1.712719});
    
    // axis({-2.0, -2.0}, {2.0, 2.0});
    axis({-2.0, -3.425438 + 1.425438}, {2.0, 3.425438 + 1.425438});
    
    // axis({-2.3, -2.3}, {2.3, 2.3});

    ParticleSystem ps{min_bnd, max_bnd};

    const VectorConstView<float> x = ps.getXView();
    const VectorConstView<float> y = ps.getYView();

    std::vector<properties::Color> colors;
    
    for(size_t k = 0; k < ps.groups_.size(); k++)
    {
        const float d = static_cast<float>(k) / static_cast<float>(ps.groups_.size() - 1U);
        colors.push_back(toColor(calculateColormapJetSoft(d)));
    }

    for (size_t k = 0; k < num_steps; k++)
    {
        ps.update(0.01);

        size_t idx = 0U;

        // if(k == ((num_steps * 3U) / 4U))
        // {
        //     ps.changeDensity();
        // }

        for(const auto grp : ps.groups_)
        {
            const size_t num_particles = grp->GetParticleCount();

            const size_t offset = grp->GetBufferIndex();

            const auto col = colors[idx];

            const VectorConstView<float> xg = VectorConstView<float>{x.data() + offset, num_particles};
            const VectorConstView<float> yg = VectorConstView<float>{y.data() + offset, num_particles};

            scatter(xg, yg, properties::ScatterStyle::DISC, properties::PointSize(10), col, properties::Silhouette{0U, 0U, 0U, 0.1f});
            

            idx++;
        }

        flushCurrentElement();
        softClearView();
    }
}

}  // namespace particles_density
