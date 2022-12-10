#include "apputils/InputHandler.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Joystick.hpp>
#include <cassert>
#include <spdlog/spdlog.h>

namespace apputils {
constexpr auto AXIS_DEADZONE_LOWER = 7.5f;
constexpr auto AXIS_DEADZONE_UPPER = 90.0f;

PadType InputHandler::m_padType { PadType::Xbox_Pad };
sf::Vector2f InputHandler::m_mousePositionWorld;
sf::Vector2i InputHandler::m_mousePositionScreen;

std::set<sf::Keyboard::Key> InputHandler::m_keyboardPressed;
std::set<sf::Keyboard::Key> InputHandler::m_keyboardHeld;

std::set<sf::Mouse::Button> InputHandler::m_mouseButtonPressed;
std::set<sf::Mouse::Button> InputHandler::m_mouseButtonReleased;
std::set<sf::Mouse::Button> InputHandler::m_mouseButtonHeld;

std::unordered_map<JoystickAxis, float> InputHandler::m_JoystickAxisPositions;
std::set<JoystickButton> InputHandler::m_joystickButtonPressed;
std::set<JoystickButton> InputHandler::m_joystickButtonReleased;
std::set<JoystickButton> InputHandler::m_joystickButtonHeld;

bool InputHandler::m_joystickConnected = false;

float get_normalized_axis_value(float pos)
{
    float pos_abs = fabsf(pos);
    const float sign = pos_abs / pos;
    if (pos_abs > AXIS_DEADZONE_LOWER) {
        pos_abs = std::min(AXIS_DEADZONE_UPPER, pos_abs);
        pos_abs += (100.0f - AXIS_DEADZONE_UPPER);
        pos_abs /= 100.0f;
        pos_abs *= sign;
        return pos_abs;
    }
    return 0.0f;
}

JoystickButton id_to_joystick_button(std::uint32_t id)
{
    switch (id) {
    case 0:
        return JoystickButton::Square;
        break;
    case 1:
        return JoystickButton::Cross;
        break;
    case 2:
        return JoystickButton::Circle;
        break;
    case 3:
        return JoystickButton::Triangle;
        break;
    case 4:
        return JoystickButton::L1;
        break;
    case 5:
        return JoystickButton::R1;
        break;
    case 6:
        return JoystickButton::L2;
        break;
    case 7:
        return JoystickButton::R2;
        break;
    case 8:
        return JoystickButton::Share;
        break;
    case 9:
        return JoystickButton::Options;
        break;
    case 10:
        return JoystickButton::Left_Stick_Click;
        break;
    case 11:
        return JoystickButton::Right_Stick_Click;
        break;
    case 12:
        // ps4 button but we don't use it yet
        break;
    case 13:
        // Trackpad on PS4 but we don't use it (yet)
        break;
    }

    return JoystickButton::Joystick_Button_Max;
}

void InputHandler::Update()
{
    m_keyboardPressed.clear();
    m_mouseButtonPressed.clear();
    m_mouseButtonReleased.clear();
    m_joystickButtonPressed.clear();
    m_joystickButtonReleased.clear();

    // On the first update we need to force a setup on the joystick axis
    // positions
    if (m_JoystickAxisPositions.empty()) {
        for (std::size_t i { 0 }; i < static_cast<std::size_t>(JoystickAxis::Joystick_Axis_Max); ++i) {
            m_JoystickAxisPositions[static_cast<JoystickAxis>(i)] = 0.f;
        }
    }
}

void InputHandler::HandleEvent(sf::Event& event, sf::RenderWindow& window)
{
    HandleKeyboardAndMouse(event, window);
    HandleGenericJoystickEvents(event);
    HandleDS4(event);
    // switch (m_padType) {
    // case PadType::Xbox_Pad:
    //	handleXbox(event);
    //	break;

    // case PadType::DS4_Pad:
    //	handleDS4(event);
    //	break;
    // default:
    //	assert(false);
    //	break;
    // }
}

auto InputHandler::KeyPressed(sf::Keyboard::Key key) -> bool
{
    const auto present = m_keyboardPressed.find(key);
    return present != m_keyboardPressed.end();
}

auto InputHandler::KeyHeld(sf::Keyboard::Key key) -> bool
{
    const auto present = m_keyboardHeld.find(key);
    return present != m_keyboardHeld.end();
}

auto InputHandler::MousePressed(sf::Mouse::Button button) -> bool
{
    const auto present = m_mouseButtonPressed.find(button);
    return present != m_mouseButtonPressed.end();
}

auto InputHandler::MouseReleased(sf::Mouse::Button button) -> bool
{
    const auto present = m_mouseButtonReleased.find(button);
    return present != m_mouseButtonReleased.end();
}

auto InputHandler::MouseHeld(sf::Mouse::Button button) -> bool
{
    const auto present = m_mouseButtonHeld.find(button);
    return present != m_mouseButtonHeld.end();
}

auto InputHandler::JoystickButtonPressed(JoystickButton button) -> bool
{
    const auto present = m_joystickButtonPressed.find(button);
    return present != m_joystickButtonPressed.end();
}

auto InputHandler::JoystickButtonReleased(JoystickButton button) -> bool
{
    const auto present = m_joystickButtonReleased.find(button);
    return present != m_joystickButtonReleased.end();
}

auto InputHandler::JoystickButtonHeld(JoystickButton button) -> bool
{
    const auto present = m_joystickButtonHeld.find(button);
    return present != m_joystickButtonHeld.end();
}

auto InputHandler::GetMouseWorldPosition() -> sf::Vector2f { return m_mousePositionWorld; }

auto InputHandler::GetMouseScreenPosition() -> sf::Vector2i { return m_mousePositionScreen; }

auto InputHandler::GetJoystickAxisPosition(JoystickAxis axis) -> float
{
    const auto result = m_JoystickAxisPositions.find(axis);
    if (result == m_JoystickAxisPositions.end())
        throw std::runtime_error("Invalid joystick axis passed");

    return result->second;
}

void InputHandler::HandleKeyboardAndMouse(const sf::Event& event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseMoved) {
        m_mousePositionScreen = sf::Vector2i { event.mouseMove.x, event.mouseMove.y };
        m_mousePositionWorld = window.mapPixelToCoords(m_mousePositionScreen);
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        m_mouseButtonPressed.emplace(event.mouseButton.button);
        m_mouseButtonHeld.emplace(event.mouseButton.button);
    }

    if (event.type == sf::Event::MouseButtonReleased) {
        auto removeIt = m_mouseButtonHeld.find(event.mouseButton.button);
        if (removeIt != m_mouseButtonHeld.end())
            m_mouseButtonHeld.erase(removeIt);

        m_mouseButtonReleased.emplace(event.mouseButton.button);
    }

    // Keyboard pressed
    if (event.type == sf::Event::KeyPressed) {
        m_keyboardPressed.emplace(event.key.code);
        m_keyboardHeld.emplace(event.key.code);
    }

    // Keyboard released
    if (event.type == sf::Event::KeyReleased) {
        auto removeIt = m_keyboardHeld.find(event.key.code);
        if (removeIt != m_keyboardHeld.end())
            m_keyboardHeld.erase(removeIt);
    }
}

void InputHandler::HandleGenericJoystickEvents(const sf::Event& event)
{
    if (sf::Joystick::isConnected(0)) {
        m_joystickConnected = true;
        auto id = sf::Joystick::getIdentification(0);
        if (id.vendorId == 1118) {
            m_padType = PadType::Xbox_Pad;
        } else if (id.vendorId == 1356) {
            m_padType = PadType::DS4_Pad;
        }
    } else {
        m_joystickConnected = false;
    }
}
void InputHandler::HandleDS4(const sf::Event& event)
{
    if (!m_joystickConnected)
        return;

    if (event.type == sf::Event::JoystickMoved) {
        // Currently we don't support any joystick index
        // greater than 0. (No multiple pad support)
        // so if we encounter a joystick id > 0, we exit
        if (event.joystickMove.joystickId != 0) {
            return;
        }
        HandleJoystickAxisDS4(event);
    }

    if (event.type == sf::Event::JoystickButtonPressed) {
        if (event.joystickMove.joystickId != 0) {
            return;
        }

        const auto button { id_to_joystick_button(event.joystickButton.button) };
        m_joystickButtonPressed.emplace(button);
        m_joystickButtonHeld.emplace(button);
    }

    if (event.type == sf::Event::JoystickButtonReleased) {
        const auto button { id_to_joystick_button(event.joystickButton.button) };
        auto removeIt = m_joystickButtonHeld.find(button);
        if (removeIt != m_joystickButtonHeld.end())
            m_joystickButtonHeld.erase(removeIt);

        m_joystickButtonReleased.emplace(button);
    }
}

void InputHandler::HandleJoystickAxisDS4(const sf::Event& event)
{
    constexpr auto getTriggerNormalizedValue
        = [](float position) -> float { return get_normalized_axis_value(position + 100.f); };

    switch (event.joystickMove.axis) {
    case sf::Joystick::Axis::X:
        m_JoystickAxisPositions[JoystickAxis::Left_Joystick_Horizontal]
            = get_normalized_axis_value(event.joystickMove.position);
        break;
    case sf::Joystick::Axis::Y:
        m_JoystickAxisPositions[JoystickAxis::Left_Joystick_Vertical]
            = get_normalized_axis_value(event.joystickMove.position);
        break;
    case sf::Joystick::Axis::Z:
        m_JoystickAxisPositions[JoystickAxis::Right_Joystick_Horizontal]
            = get_normalized_axis_value(event.joystickMove.position);
        break;
    case sf::Joystick::Axis::R:
        m_JoystickAxisPositions[JoystickAxis::Right_Joystick_Vertical]
            = get_normalized_axis_value(event.joystickMove.position);
        break;
    case sf::Joystick::Axis::U:
        m_JoystickAxisPositions[JoystickAxis::L2] = getTriggerNormalizedValue(event.joystickMove.position);
        break;
    case sf::Joystick::Axis::V:
        m_JoystickAxisPositions[JoystickAxis::R2] = getTriggerNormalizedValue(event.joystickMove.position);
        break;
    case sf::Joystick::Axis::PovX:
        m_JoystickAxisPositions[JoystickAxis::Directional_Pad_Horizontal]
            = get_normalized_axis_value(event.joystickMove.position);
        break;
    case sf::Joystick::Axis::PovY:
        m_JoystickAxisPositions[JoystickAxis::Directional_Pad_Vertical]
            = get_normalized_axis_value(event.joystickMove.position);
        break;
    }
}
} // namespace apputils
