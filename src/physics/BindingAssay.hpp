#pragma once

#include "../core/Types.hpp"
#include "../core/Constants.hpp"
#include "Electrostatics.hpp"
#include <string>

namespace phageforge::physics {

/**
 * @brief Binding assay for phage-bacteria interaction scoring
 * 
 * Phase 3: Full implementation with:
 * - Binding energy → game score conversion
 * - Infection threshold checking
 * - Human-readable binding descriptions
 * - Binding strength categorization
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
     * @param energy Binding energy in kJ/mol (negative = favorable)
     * @return Game score (0-100, higher = better binding)
     */
    [[nodiscard]] static float scoreBinding(double energy) {
        // Map energy to score: -30 kJ/mol = 100, 0 kJ/mol = 0
        const double min_energy = -30.0;  // kJ/mol
        const double max_energy = 0.0;    // kJ/mol
        
        // Clamp energy to range
        double clamped = std::max(min_energy, std::min(max_energy, energy));
        
        // Normalize to 0-100
        double score = (clamped - min_energy) / (max_energy - min_energy) * 100.0;
        
        return static_cast<float>(score);
    }
    
    /**
     * @brief Check if binding energy is sufficient for infection
     * 
     * @param energy Binding energy in kJ/mol
     * @return true if infection is possible
     */
    [[nodiscard]] static bool isInfection(double energy) {
        return energy < core::constants::INFECTION_THRESHOLD;
    }
    
    /**
     * @brief Get a human-readable binding strength description
     * 
     * @param energy Binding energy in kJ/mol
     * @return Description string
     */
    [[nodiscard]] static std::string getBindingDescription(double energy) {
        if (energy < -25.0) return "⚡ Extremely Strong - High Infection Probability";
        if (energy < -20.0) return "💪 Very Strong - Good Infection Probability";
        if (energy < -15.0) return "✅ Strong - Likely Infection";
        if (energy < -10.0) return "⚠️ Moderate - Possible Infection";
        if (energy < -5.0)  return "🔵 Weak - Unlikely Infection";
        if (energy < 0.0)   return "📉 Very Weak - Low Chance";
        return "❌ No Binding - No Infection";
    }
    
    /**
     * @brief Get the binding strength category
     * 
     * @param energy Binding energy in kJ/mol
     * @return Category (0-5)
     */
    [[nodiscard]] static int getBindingCategory(double energy) {
        if (energy < -25.0) return 6;
        if (energy < -20.0) return 5;
        if (energy < -15.0) return 4;
        if (energy < -10.0) return 3;
        if (energy < -5.0)  return 2;
        if (energy < 0.0)   return 1;
        return 0;
    }
    
    /**
     * @brief Get binding category name
     */
    [[nodiscard]] static std::string getBindingCategoryName(int category) {
        switch(category) {
            case 6: return "Extremely Strong";
            case 5: return "Very Strong";
            case 4: return "Strong";
            case 3: return "Moderate";
            case 2: return "Weak";
            case 1: return "Very Weak";
            default: return "No Binding";
        }
    }
    
    /**
     * @brief Calculate infection probability from binding energy
     * 
     * @param energy Binding energy in kJ/mol
     * @return Probability (0.0 - 1.0)
     */
    [[nodiscard]] static double infectionProbability(double energy) {
        // Sigmoid function mapping energy to probability
        // At -15 kJ/mol, probability = 0.5
        // At -30 kJ/mol, probability = 0.95
        // At 0 kJ/mol, probability = 0.05
        const double mid = -15.0;   // kJ/mol
        const double slope = 0.25;   // 1/kJ/mol
        
        double prob = 1.0 / (1.0 + std::exp(slope * (energy - mid)));
        return prob;
    }
};

} // namespace phageforge::physics
