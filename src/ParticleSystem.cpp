#include "ParticleSystem.hpp"
#include <algorithm>
#include <apputils/RNG.hpp>
#include <spdlog/spdlog.h>

constexpr size_t MAX_PARTICLE_COUNT = 1000;

ParticleSystem::ParticleSystem(const Configuration& configuration)
    : m_config(configuration)
    , m_vertices(sf::PrimitiveType::Triangles)
{
    m_particles.resize(MAX_PARTICLE_COUNT);
}

void ParticleSystem::update(const sf::Time& dt)
{
    m_emissionTimer += dt;
    if (readyToEmitParticle()) {
        emitParticle();
        // TODO: For a starting point it's fine to just emit and set the timer to zero
        // but ideally we would do some modulo to retain the left over time so we're always
        // entirely accurate on the timing.
        m_emissionTimer = sf::Time::Zero;
    }

    stepParticles(dt);

    // Cull particles which have exceeded their lifetime
    std::for_each(m_particles.begin(), m_particles.end(), [](Particle& p) -> void {
        if (p.activeTime > p.maxActiveTime) {
            p.active = false;
        }
    });
}

void ParticleSystem::draw(sf::RenderTarget& target, const sf::RenderStates& states) const
{
    target.draw(m_vertices, states);
}

auto ParticleSystem::readyToEmitParticle() const -> bool { return m_emissionTimer > m_config.spawnFrequency; }

void ParticleSystem::emitParticle()
{
    // Find an inactive particle we can use
    auto particleIt = std::find_if(
        m_particles.begin(), m_particles.end(), [](const Particle& particle) -> bool { return !particle.active; });

    if (particleIt == m_particles.end())
        return;

    auto& particle = *particleIt;

    particle.active = true;
    particle.position = m_config.centrePosition;
    particle.direction = getRandomEmissionDirection();
    particle.speed = m_config.speed.start;
    particle.activeTime = sf::Time::Zero;
    particle.maxActiveTime = sf::seconds(
        apputils::RNG::realWithinRange(m_config.lifetime.start.asSeconds(), m_config.lifetime.end.asSeconds()));
}

auto ParticleSystem::getRandomEmissionDirection() const -> sf::Vector2f
{
    const auto angle = sf::degrees(apputils::RNG::realWithinRange(m_config.emissionAngle.start.asDegrees(),
                                                                  m_config.emissionAngle.end.asDegrees()));
    return { 1.f, angle };
}

void ParticleSystem::stepParticles(const sf::Time& dt)
{
    m_vertices.clear();

    const auto numberOfParticles
        = std::count_if(m_particles.begin(), m_particles.end(), [](const Particle& p) { return p.active; });

    spdlog::debug("Number of partilces this frame {}", numberOfParticles);

    for (auto& p : m_particles) {
        if (!p.active)
            continue;

        p.activeTime += dt;
        p.position += (p.speed * p.direction) * dt.asSeconds();
        constructParticleGeometry(p);
    }
}

void ParticleSystem::constructParticleGeometry(const Particle& p)
{
    // Really shouldn't end up in this state...
    assert(p.active);

    const auto halfBounds = m_config.size * 0.5f;
    std::array<sf::Vertex, 4> quadVertices;

    quadVertices[0].position = p.position + sf::Vector2f { -halfBounds.x, -halfBounds.y };
    quadVertices[1].position = p.position + sf::Vector2f { halfBounds.x, -halfBounds.y };
    quadVertices[2].position = p.position + sf::Vector2f { halfBounds.x, halfBounds.y };
    quadVertices[3].position = p.position + sf::Vector2f { -halfBounds.x, halfBounds.y };

    m_vertices.append(quadVertices[0]);
    m_vertices.append(quadVertices[1]);
    m_vertices.append(quadVertices[2]);
    m_vertices.append(quadVertices[2]);
    m_vertices.append(quadVertices[3]);
    m_vertices.append(quadVertices[0]);
}
