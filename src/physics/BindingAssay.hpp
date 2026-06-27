#pragma once

#include "../core/Types.hpp"
#include "../core/Constants.hpp"

namespace phageforge::physics {

/**
 * @brief Binding assay for phage-bacteria interaction scoring
 * 
 * For Phase 1: This is a stub. Phase 3 will implement:
 * - Binding energy calculation from electrostatics
 * - Score conversion for game mechanics
 * - Infection threshold checking
 */
class BindingAssay {
public:
    BindingAssay() = default;
    ~BindingAssay() = default;
    
    /**
     * @brief Convert binding energy to a game score
     * 
     * More negative energy = stronger binding = higher score
     * 
     * @param energy Binding energy in kJ/mol
     * @return Game score (positive = good binding)
     */
    [[nodiscard]] static float scoreBinding(float energy) {
        // Phase 3: Convert energy to a meaningful game score
        // For now, return the negative of energy (more negative = higher score)
        return -energy;
    }
    
    /**
     * @brief Check if binding energy is sufficient for infection
     * 
     * @param energy Binding energy in kJ/mol
     * @return true if infection is possible
     */
    [[nodiscard]] static bool isInfection(float energy) {
        // Check if binding energy is below threshold (strong enough)
        return energy < core::constants::INFECTION_THRESHOLD;
    }
    
    /**
     * @brief Get a human-readable binding strength description
     * 
     * @param energy Binding energy in kJ/mol
     * @return Description string
     */
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
