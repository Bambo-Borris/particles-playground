#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Time.hpp>
#include <apputils/ColourUtils.hpp>
#include <vector>

template <typename T>
struct Range {
    T start;
    T end;
};

class ParticleSystem : public sf::Drawable {
public:
    struct Configuration {
        Range<sf::Colour> colour = { {}, {} };
        Range<sf::Angle> emissionAngle = { sf::Angle::Zero, sf::Angle::Zero };
        Range<float> alpha = { 0.f, 0.f };
        Range<float> scale = { 0.f, 0.f };
        Range<float> speed = { 0.f, 0.f };
        Range<sf::Time> lifetime { {}, {} };

        sf::Vector2f size = {};
        sf::Vector2f centrePosition = {};
        sf::Time spawnFrequency = {};
    };

    ParticleSystem(const Configuration& configuration);

    void update(const sf::Time& dt);

protected:
    virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;

private:
    struct Particle {
        sf::Texture* texture { nullptr };
        sf::Vector2f direction;
        sf::Vector2f position;
        float speed = { 0.f };
        bool active { false };
        sf::Time activeTime;
        sf::Time maxActiveTime;
    };

    auto readyToEmitParticle() const -> bool;
    auto getRandomEmissionDirection() const -> sf::Vector2f;
    void emitParticle();
    void stepParticles(const sf::Time& dt);
    void constructParticleGeometry(const Particle& p);

    Configuration m_config;

    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;

    sf::Time m_emissionTimer;
};
