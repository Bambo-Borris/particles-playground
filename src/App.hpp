#pragma once

#include "ParticleSystem.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

class App {
public:
    App();
    ~App();

    void run();

private:
    sf::RenderWindow m_window;
    ParticleSystem m_particleSystem;
};
