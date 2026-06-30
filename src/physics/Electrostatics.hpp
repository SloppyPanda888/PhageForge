#pragma once

#include "../core/Types.hpp"
#include "../core/Constants.hpp"
#include "../biology/Genome.hpp"
#include "../biology/Bacteria.hpp"
#include "../biology/AminoAcid.hpp"
#include "VectorMath.hpp"
#include <cmath>
#include <vector>
#include <utility>

namespace phageforge::physics {

/**
 * @brief Electrostatic interaction solver using Debye-Hückel theory
 * 
 * Phase 3: Full implementation with:
 * - Debye-Hückel potential for charged particles
 * - Binding energy summation across all phage-bacteria pairs
 * - 3D positioning of amino acids and receptors
 * - Ionic strength and temperature dependence
 */
class ElectrostaticsSolver {
public:
    ElectrostaticsSolver();
    ~ElectrostaticsSolver() = default;
    
    /**
     * @brief Compute binding energy between phage and bacteria
     * 
     * @param phage The phage genome (translated to amino acids)
     * @param bacteria The bacterial strain with surface receptors
     * @return Binding energy in kJ/mol (negative = favorable)
     */
    [[nodiscard]] double computeBindingEnergy(
        const biology::Genome& phage,
        const biology::BacterialStrain& bacteria) const;
    
    /**
     * @brief Compute detailed interaction map
     * 
     * @param phage The phage genome
     * @param bacteria The bacterial strain
     * @return Vector of (amino_acid_index, receptor_index, energy) pairs
     */
    [[nodiscard]] std::vector<std::tuple<size_t, size_t, double>> 
    getInteractionMap(
        const biology::Genome& phage,
        const biology::BacterialStrain& bacteria) const;
    
    /**
     * @brief Debye-Hückel potential between two charged particles
     * 
     * @param q1 Charge of particle 1 (elementary charge units)
     * @param q2 Charge of particle 2 (elementary charge units)
     * @param distance Distance between particles (nm)
     * @return Electrostatic potential energy (kJ/mol)
     */
    [[nodiscard]] static double debyeHuckelPotential(
        double q1, double q2, double distance);
    
    /**
     * @brief Compute inverse Debye length from ionic strength
     * 
     * @param ionic_strength Ionic strength in M
     * @param temperature Temperature in K
     * @return Inverse Debye length in nm⁻¹
     */
    [[nodiscard]] static double computeDebyeLength(
        double ionic_strength, 
        double temperature);
    
    /**
     * @brief Get the positions of amino acids in the phage tail
     * 
     * @param phage The phage genome
     * @return Vector of (position, charge) pairs
     */
    [[nodiscard]] std::vector<std::pair<core::Point3D, double>> 
    getPhageCharges(const biology::Genome& phage) const;
    
    /**
     * @brief Get the positions of receptors on the bacterial surface
     * 
     * @param bacteria The bacterial strain
     * @return Vector of (position, charge) pairs
     */
    [[nodiscard]] std::vector<std::pair<core::Point3D, double>> 
    getBacterialCharges(const biology::BacterialStrain& bacteria) const;
    
    // --- Settings ---
    void setIonicStrength(double ionic_strength);
    void setTemperature(double temperature);
    void setCutoffDistance(double cutoff_nm);
    void setMinDistance(double min_nm);
    
    [[nodiscard]] double getIonicStrength() const { return m_ionic_strength; }
    [[nodiscard]] double getTemperature() const { return m_temperature; }
    [[nodiscard]] double getDebyeLengthInverse() const { return m_debye_length_inverse; }
    [[nodiscard]] double getDebyeLength() const { return 1.0 / m_debye_length_inverse; }
    
private:
    double m_ionic_strength;
    double m_temperature;
    double m_debye_length_inverse;
    double m_cutoff_distance;
    double m_min_distance;
    
    // Helper: convert charge from elementary units to Coulombs
    [[nodiscard]] static double elementaryToCoulomb(double charge_e) {
        return charge_e * core::constants::E_CHARGE;
    }
    
    // Helper: convert energy from Joules to kJ/mol
    [[nodiscard]] static double joulesToKJPerMol(double joules) {
        return joules * core::constants::AVOGADRO / 1000.0;
    }
};

} // namespace phageforge::physics
