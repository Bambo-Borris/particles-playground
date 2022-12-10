#include "App.hpp"
#include "apputils/InputHandler.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

constexpr auto WINDOW_TITLE { "Particle Playground - v0.1.0" };

App::App()
{
    m_window.create(sf::VideoMode({ 1024, 768 }), WINDOW_TITLE, sf::Style::Default ^ sf::Style::Resize);
    m_window.setFramerateLimit(144);
}

App::~App() { }

void App::run()
{

    while (m_window.isOpen()) {
        sf::Event event;
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

        m_window.clear();
        m_window.display();
    }
}
