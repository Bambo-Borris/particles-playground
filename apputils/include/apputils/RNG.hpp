#pragma once

#include <random>
#include <type_traits>

namespace apputils {

class RNG {
public:
    static auto realWithinRange(float start, float end) -> float
    {
        std::uniform_real_distribution<float> dist { start, end };
        return dist(m_randomEngine);
    }

    static auto intWithinRange(std::int32_t start, std::int32_t end) -> std::int32_t 
    {
        std::uniform_int_distribution dist { start, end };
        return dist(m_randomEngine);
    }

private:
    static std::random_device m_randomDevice;
    static std::default_random_engine m_randomEngine;
};
} // namespace apputils
