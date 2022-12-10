#pragma once

#include <apputils/MathsUtils.hpp>
#include <apputils/RNG.hpp>

#include <SFML/Graphics/Color.hpp>
#include <array>

// I'm serious. This is required.
namespace sf {
using Colour = Color;
}

namespace apputils {

// Returned as:
// col.r = [0]
// col.g = [1]
// col.b = [2]
// col.a = [3]
inline std::array<float, 4> ColourToFloatArray(const sf::Colour& colour)
{
    std::array<float, 4> out;
    out[0] = static_cast<float>(colour.r) / 255.f;
    out[1] = static_cast<float>(colour.g) / 255.f;
    out[2] = static_cast<float>(colour.b) / 255.f;
    out[3] = static_cast<float>(colour.a) / 255.f;

    return out;
}

inline sf::Colour FloatArrayToColour(const std::array<float, 4>& colour)
{
    sf::Colour out;
    out.r = static_cast<std::uint8_t>(colour[0] * 255.f);
    out.g = static_cast<std::uint8_t>(colour[1] * 255.f);
    out.b = static_cast<std::uint8_t>(colour[2] * 255.f);
    out.a = static_cast<std::uint8_t>(colour[3] * 255.f);

    return out;
}

inline sf::Colour LerpColour(const sf::Color& start, const sf::Color& end, float t)
{
    sf::Colour out;
    const auto startArray = ColourToFloatArray(start);
    const auto endArray = ColourToFloatArray(end);

    std::array<float, 4> result;
    for (std::size_t i = 0; i < 4; ++i) {
        result[i] = Lerp(startArray[i], endArray[i], t);
    }

    return FloatArrayToColour(result);
}

inline sf::Colour GenerateRandomColour()
{
    auto red = RNG::intWithinRange(0, 256);
    auto green = RNG::intWithinRange(0, 256);
    auto blue = RNG::intWithinRange(0, 256);

    red = (red + 255) / 2;
    green = (green + 255) / 2;
    blue = (blue + 255) / 2;

    return { static_cast<std::uint8_t>(red), static_cast<std::uint8_t>(green), static_cast<std::uint8_t>(blue) };
}
} // namespace apputils
