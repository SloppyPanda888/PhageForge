#pragma once

#include "../core/Types.hpp"
#include "../core/Constants.hpp"
#include "../biology/Genome.hpp"
#include "../biology/Bacteria.hpp"
#include "../biology/AminoAcid.hpp"
#include <cmath>

namespace phageforge::physics {

/**
 * @brief Electrostatic interaction solver for phage-bacteria binding
 */
class ElectrostaticsSolver {
public:
    ElectrostaticsSolver() = default;
    ~ElectrostaticsSolver() = default;
    
    /**
     * @brief Compute binding energy between phage and bacteria
     */
    [[nodiscard]] static float computeBindingEnergy(
        const biology::Genome& /*phage*/, 
        const biology::BacterialStrain& /*bacteria*/) {
        // Phase 1 stub - Phase 3 will implement full Debye-Hückel
        return 0.0f;
    }
    
    /**
     * @brief Debye-Hückel potential between two charged particles
     */
    [[nodiscard]] static float debyeHuckelPotential(
        float /*q1*/, float /*q2*/, float /*distance*/) {
        return 0.0f;
    }
    
    /**
     * @brief Set the ionic strength of the solution
     */
    static void setIonicStrength(float /*ionic_strength*/) {
        // Phase 3: Update Debye length calculation
    }
    
    /**
     * @brief Set the temperature of the solution
     */
    static void setTemperature(float /*temperature*/) {
        // Phase 3: Update Debye length calculation
    }
};

} // namespace phageforge::physics
