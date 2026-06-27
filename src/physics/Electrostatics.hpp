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
 * 
 * For Phase 1: This is a stub. Phase 3 will implement:
 * - Debye-Hückel potential for charged particles
 * - Full electrostatic field calculation
 * - Binding energy summation across all phage-bacteria pairs
 */
class ElectrostaticsSolver {
public:
    ElectrostaticsSolver() = default;
    ~ElectrostaticsSolver() = default;
    
    /**
     * @brief Compute binding energy between phage and bacteria
     * 
     * Phase 1: Stub implementation returning 0.0
     * Phase 3: Full Debye-Hückel implementation
     * 
     * @param phage The phage genome (translated to amino acids)
     * @param bacteria The bacterial strain with surface receptors
     * @return Binding energy in kJ/mol (negative = favorable)
     */
    [[nodiscard]] static float computeBindingEnergy(
        const biology::Genome& /*phage*/, 
        const biology::BacterialStrain& /*bacteria*/) {
        // Phase 1 stub - Phase 3 will implement full Debye-Hückel
        return 0.0f;
    }
    
    /**
     * @brief Debye-Hückel potential between two charged particles
     * 
     * Phase 3 implementation:
     * U(r) = (q1 * q2 / (4 * pi * eps0 * epsr)) * (exp(-kappa * r) / r)
     * 
     * @param q1 Charge of particle 1 (elementary charge units)
     * @param q2 Charge of particle 2 (elementary charge units)
     * @param distance Distance between particles (nm)
     * @return Electrostatic potential energy (kJ/mol)
     */
    [[nodiscard]] static float debyeHuckelPotential(
        float /*q1*/, float /*q2*/, float /*distance*/) {
        // Phase 3 implementation
        return 0.0f;
    }
    
    /**
     * @brief Set the ionic strength of the solution
     * 
     * @param ionic_strength Molar concentration (M)
     */
    static void setIonicStrength(float ionic_strength) {
        // Phase 3: Update Debye length calculation
        (void)ionic_strength;  // Suppress unused parameter warning
    }
    
    /**
     * @brief Set the temperature of the solution
     * 
     * @param temperature Temperature in Kelvin
     */
    static void setTemperature(float temperature) {
        // Phase 3: Update Debye length calculation
        (void)temperature;  // Suppress unused parameter warning
    }
};

} // namespace phageforge::physics
