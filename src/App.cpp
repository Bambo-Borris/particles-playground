#include "App.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <apputils/InputHandler.hpp>
#include <spdlog/spdlog.h>

constexpr auto WINDOW_TITLE { "Particle Playground - v0.1.0" };

App::App()
    : m_particleSystem({})
{
    m_window.create(sf::VideoMode({ 1024, 768 }), WINDOW_TITLE, sf::Style::Default ^ sf::Style::Resize);
    m_window.setFramerateLimit(144);

    spdlog::set_level(spdlog::level::debug);
}

App::~App() { }

void App::run()
{
    // For testing purposes we'll setup a basic explosion effect
    ParticleSystem::Configuration config;
    config.alpha = { 1.f, 0.f };
    config.scale = { 1.f, 2.f };
    config.colour = { sf::Color::White, sf::Color::White };
    config.emissionAngle = { sf::degrees(0), sf::degrees(359.f) };
    config.speed = { 200.f, 50.f };
    config.spawnFrequency = sf::seconds(0.001f);
    config.lifetime = { sf::seconds(0.2f), sf::seconds(0.8f) };

    config.size = { 10.f, 10.f };
    config.centrePosition = { sf::Vector2f(m_window.getSize()) * 0.5f };

    // Now reconstruct our particle system with the config we just supplied
    m_particleSystem = { config };

    sf::Clock deltaClock;
    while (m_window.isOpen()) {
        sf::Event event;
        const auto dt = deltaClock.restart();
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                m_window.close();

                // Don't want to close the window with escape unless in debug
#ifdef PARTICLE_PLAYGROUND_DEBUG
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    m_window.close();
            }
#endif
            apputils::InputHandler::HandleEvent(event, m_window);
        }

        apputils::InputHandler::Update();
        m_particleSystem.update(dt);

        m_window.clear();
        m_window.draw(m_particleSystem);
        m_window.display();
    }
}
