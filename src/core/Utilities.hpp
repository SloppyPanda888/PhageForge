#pragma once

#include "Types.hpp"
#include <optional>
#include <string>
#include <cmath>

namespace phageforge::core {

// --- String Conversions for Amino Acids ---
[[nodiscard]] std::string aminoAcidToString(AminoAcidCode aa) noexcept;
[[nodiscard]] std::optional<AminoAcidCode> stringToAminoAcid(const std::string& three_letter) noexcept;

// --- Math Utilities ---
template<typename T>
[[nodiscard]] constexpr T clamp(T value, T min_val, T max_val) noexcept {
    return (value < min_val) ? min_val : (value > max_val) ? max_val : value;
}

template<typename T>
[[nodiscard]] constexpr T lerp(T a, T b, float t) noexcept {
    return a + (b - a) * t;
}

template<typename T>
[[nodiscard]] constexpr T square(T value) noexcept {
    return value * value;
}

// --- Random Utilities (for Phase 1 testing) ---
[[nodiscard]] float randomFloat(float min_val, float max_val, std::mt19937& rng);

} // namespace phageforge::core
