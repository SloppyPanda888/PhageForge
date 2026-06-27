#pragma once

#include "../core/Types.hpp"
#include "../core/Constants.hpp"
#include <string>

namespace phageforge::physics {

/**
 * @brief Binding assay for phage-bacteria interaction scoring
 */
class BindingAssay {
public:
    BindingAssay() = default;
    ~BindingAssay() = default;
    
    [[nodiscard]] static float scoreBinding(float energy) {
        return -energy;
    }
    
    [[nodiscard]] static bool isInfection(float energy) {
        return energy < core::constants::INFECTION_THRESHOLD;
    }
    
    [[nodiscard]] static std::string getBindingDescription(float energy) {
        if (energy < -20.0f) return "Extremely Strong";
        if (energy < -15.0f) return "Very Strong";
        if (energy < -10.0f) return "Strong";
        if (energy < -5.0f)  return "Moderate";
        if (energy < 0.0f)   return "Weak";
        return "No Binding";
    }
};

} // namespace phageforge::physics
