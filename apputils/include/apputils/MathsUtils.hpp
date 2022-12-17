#pragma once

#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <cmath>

namespace apputils {
constexpr auto PI { 3.141592f };

inline float Lerp(float v0, float v1, float t) { return (1 - t) * v0 + t * v1; }

inline float EaseOutElastic(float v0, float v1, float t)
{
    constexpr auto c4 { (2.f * PI) / 3.f };
    const auto distance { v1 - v0 };
    const auto result { t == 0.f ? 0.f : t == 1 ? 1 : std::pow(2.f, -10.f * t) * std::sin((t * 10.f - 0.75f) * c4) + 1.f };
    return v0 + float(distance * result);
}
} // namespace apputils
