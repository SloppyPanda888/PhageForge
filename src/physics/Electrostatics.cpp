#include "Electrostatics.hpp"
#include "../biology/AminoAcid.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

namespace phageforge::physics {

ElectrostaticsSolver::ElectrostaticsSolver()
    : m_ionic_strength(core::constants::IONIC_STRENGTH),
      m_temperature(core::constants::TEMPERATURE),
      m_cutoff_distance(core::constants::MAX_INTERACTION_DISTANCE),
      m_min_distance(core::constants::MIN_DISTANCE) {
    // Compute Debye length from ionic strength
    m_debye_length_inverse = computeDebyeLength(m_ionic_strength, m_temperature);
}

double ElectrostaticsSolver::debyeHuckelPotential(double q1, double q2, double distance) {
    // Avoid singularities
    if (distance < core::constants::MIN_DISTANCE) {
        distance = core::constants::MIN_DISTANCE;
    }
    
    // Cutoff for long-range interactions
    if (distance > core::constants::MAX_INTERACTION_DISTANCE) {
        return 0.0;
    }
    
    // Constants in SI units
    constexpr double k_e = 1.0 / (4.0 * 3.141592653589793 * core::constants::EPSILON_0);
    
    // Convert charges from elementary units to Coulombs
    double q1_c = q1 * core::constants::E_CHARGE;
    double q2_c = q2 * core::constants::E_CHARGE;
    
    // Convert distance from nm to m
    double r_m = distance * 1e-9;
    double kappa_m = core::constants::DEBYE_LENGTH_INVERSE * 1e9;  // nm⁻¹ → m⁻¹
    
    // Debye-Hückel potential in Joules
    double potential_J = k_e * (q1_c * q2_c / r_m) * std::exp(-kappa_m * r_m);
    
    // Convert to kJ/mol
    return joulesToKJPerMol(potential_J);
}

double ElectrostaticsSolver::computeDebyeLength(double ionic_strength, double temperature) {
    // Ionic strength in M (mol/L) → convert to mol/m³
    double I_mol_m3 = ionic_strength * 1000.0;
    
    // Constants
    constexpr double N_A = core::constants::AVOGADRO;
    constexpr double e = core::constants::E_CHARGE;
    constexpr double eps0 = core::constants::EPSILON_0;
    constexpr double k_B = core::constants::BOLTZMANN_CONSTANT;
    constexpr double eps_r = core::constants::DIELECTRIC_CONSTANT_WATER;
    
    // Compute kappa (inverse Debye length) in m⁻¹
    double kappa_squared = (2.0 * N_A * e * e * I_mol_m3) / (eps0 * eps_r * k_B * temperature);
    double kappa = std::sqrt(kappa_squared);
    
    // Convert to nm⁻¹
    return kappa * 1e-9;
}

std::vector<std::pair<core::Point3D, double>> 
ElectrostaticsSolver::getPhageCharges(const biology::Genome& phage) const {
    std::vector<std::pair<core::Point3D, double>> charges;
    
    // Translate genome to amino acids
    auto aa_sequence = phage.translateTailFiber();
    if (aa_sequence.empty()) {
        return charges;
    }
    
    // Position amino acids along a helix
    const int num_aa = static_cast<int>(std::min(aa_sequence.size(), size_t(20)));
    charges.reserve(num_aa);
    
    // Helix parameters for phage tail fiber
    const float helix_radius = 0.5f;   // nm
    const float helix_pitch = 0.8f;    // nm
    
    // Get amino acid properties
    auto& aa_manager = biology::AminoAcidPropertiesManager::instance();
    
    for (int i = 0; i < num_aa; ++i) {
        auto aa = aa_sequence[i];
        auto props = aa_manager.getProperties(aa);
        
        // Position along helix
        float t = static_cast<float>(i) / num_aa;
        float angle = t * 2.0f * 3.14159f * 3.0f;  // 3 turns
        float z = t * helix_pitch * 10.0f;  // Total length ~8 nm
        
        core::Point3D pos{
            helix_radius * std::cos(angle),
            helix_radius * std::sin(angle),
            z
        };
        
        // Charge in elementary units
        double charge = props.net_charge_at_ph7;
        
        charges.push_back({pos, charge});
    }
    
    return charges;
}

std::vector<std::pair<core::Point3D, double>> 
ElectrostaticsSolver::getBacterialCharges(const biology::BacterialStrain& bacteria) const {
    std::vector<std::pair<core::Point3D, double>> charges;
    
    const auto& receptors = bacteria.getReceptors();
    charges.reserve(receptors.size());
    
    for (const auto& receptor : receptors) {
        core::Point3D pos = receptor.getPosition();
        double charge = receptor.getCharge();
        charges.push_back({pos, charge});
    }
    
    return charges;
}

double ElectrostaticsSolver::computeBindingEnergy(
    const biology::Genome& phage,
    const biology::BacterialStrain& bacteria) const {
    
    // Get phage charges (amino acids)
    auto phage_charges = getPhageCharges(phage);
    if (phage_charges.empty()) {
        return 0.0;
    }
    
    // Get bacterial charges (receptors)
    auto bacterial_charges = getBacterialCharges(bacteria);
    if (bacterial_charges.empty()) {
        return 0.0;
    }
    
    // Compute total binding energy
    double total_energy = 0.0;
    int num_pairs = 0;
    
    for (const auto& [phage_pos, phage_charge] : phage_charges) {
        for (const auto& [bact_pos, bact_charge] : bacterial_charges) {
            // Calculate distance
            double dist = physics::distance(phage_pos, bact_pos);
            
            // Skip if too far
            if (dist > m_cutoff_distance) continue;
            
            // Compute Debye-Hückel potential
            double energy = debyeHuckelPotential(phage_charge, bact_charge, dist);
            
            total_energy += energy;
            num_pairs++;
        }
    }
    
    // Average energy per interaction (normalize)
    if (num_pairs > 0) {
        total_energy /= static_cast<double>(num_pairs);
        // Multiply by number of interactions for total energy
        total_energy *= static_cast<double>(num_pairs);
    }
    
    return total_energy;
}

std::vector<std::tuple<size_t, size_t, double>> 
ElectrostaticsSolver::getInteractionMap(
    const biology::Genome& phage,
    const biology::BacterialStrain& bacteria) const {
    
    std::vector<std::tuple<size_t, size_t, double>> interactions;
    
    // Get phage charges
    auto phage_charges = getPhageCharges(phage);
    if (phage_charges.empty()) return interactions;
    
    // Get bacterial charges
    auto bacterial_charges = getBacterialCharges(bacteria);
    if (bacterial_charges.empty()) return interactions;
    
    interactions.reserve(phage_charges.size() * bacterial_charges.size());
    
    for (size_t i = 0; i < phage_charges.size(); ++i) {
        const auto& [phage_pos, phage_charge] = phage_charges[i];
        for (size_t j = 0; j < bacterial_charges.size(); ++j) {
            const auto& [bact_pos, bact_charge] = bacterial_charges[j];
            
            double dist = physics::distance(phage_pos, bact_pos);
            if (dist > m_cutoff_distance) continue;
            
            double energy = debyeHuckelPotential(phage_charge, bact_charge, dist);
            interactions.emplace_back(i, j, energy);
        }
    }
    
    return interactions;
}

void ElectrostaticsSolver::setIonicStrength(double ionic_strength) {
    m_ionic_strength = ionic_strength;
    m_debye_length_inverse = computeDebyeLength(ionic_strength, m_temperature);
}

void ElectrostaticsSolver::setTemperature(double temperature) {
    m_temperature = temperature;
    m_debye_length_inverse = computeDebyeLength(m_ionic_strength, temperature);
}

void ElectrostaticsSolver::setCutoffDistance(double cutoff_nm) {
    m_cutoff_distance = std::max(cutoff_nm, m_min_distance);
}

void ElectrostaticsSolver::setMinDistance(double min_nm) {
    m_min_distance = std::max(min_nm, 0.01);
}

} // namespace phageforge::physics
