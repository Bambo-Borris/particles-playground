#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <set>
#include <unordered_map>
//
// Example use of this class:
// if (InputHandler::keyPressed(sf::Keyboard::Escape))
//    DoStuff();
//
namespace apputils {
enum class PadType { Xbox_Pad, DS4_Pad };
enum class JoystickAxis { // Named as though on a PS4 since we only support gamepads
    Left_Joystick_Horizontal,
    Left_Joystick_Vertical,
    Right_Joystick_Horizontal,
    Right_Joystick_Vertical,
    L2,
    R2,
    Directional_Pad_Horizontal,
    Directional_Pad_Vertical,
    Joystick_Axis_Max
};

enum class JoystickButton { // Named as though on PS4 pad since we only support gamepads
    Cross,
    Circle,
    Square,
    Triangle,
    L1,
    R1,
    L2,
    R2,
    Share,
    Options,
    Left_Stick_Click,
    Right_Stick_Click,
    Joystick_Button_Max
};

class InputHandler {
public:
    static void Update();
    static void HandleEvent(sf::Event& event, sf::RenderWindow& window);

    static auto KeyPressed(sf::Keyboard::Key key) -> bool;
    static auto KeyHeld(sf::Keyboard::Key key) -> bool;

    static auto MousePressed(sf::Mouse::Button button) -> bool;
    static auto MouseReleased(sf::Mouse::Button button) -> bool;
    static auto MouseHeld(sf::Mouse::Button button) -> bool;

    static auto JoystickButtonPressed(JoystickButton button) -> bool;
    static auto JoystickButtonReleased(JoystickButton button) -> bool;
    static auto JoystickButtonHeld(JoystickButton button) -> bool;

    static auto GetMouseWorldPosition() -> sf::Vector2f;
    static auto GetMouseScreenPosition() -> sf::Vector2i;
    static auto GetJoystickAxisPosition(JoystickAxis axis) -> float;

private:
    static void HandleKeyboardAndMouse(const sf::Event& event, sf::RenderWindow& window);
    static void HandleGenericJoystickEvents(const sf::Event& event);
    static void HandleDS4(const sf::Event& event);
    static void HandleJoystickAxisDS4(const sf::Event& event);
    // void handleXbox(const sf::Event& event);

    static PadType m_padType;
    static sf::Vector2f m_mousePositionWorld;
    static sf::Vector2i m_mousePositionScreen;

    static std::set<sf::Keyboard::Key> m_keyboardPressed;
    static std::set<sf::Keyboard::Key> m_keyboardHeld;

    static std::set<sf::Mouse::Button> m_mouseButtonPressed;
    static std::set<sf::Mouse::Button> m_mouseButtonReleased;
    static std::set<sf::Mouse::Button> m_mouseButtonHeld;

    static std::unordered_map<JoystickAxis, float> m_JoystickAxisPositions;
    static std::set<JoystickButton> m_joystickButtonPressed;
    static std::set<JoystickButton> m_joystickButtonReleased;
    static std::set<JoystickButton> m_joystickButtonHeld;

    static bool m_joystickConnected;
};
} // namespace apputils
