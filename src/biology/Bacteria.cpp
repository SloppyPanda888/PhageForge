#include "Bacteria.hpp"
#include "Genome.hpp"
#include "AminoAcid.hpp"
#include <cmath>
#include <algorithm>

namespace phageforge::biology {

BacterialStrain::BacterialStrain() {
    // Initialize with some default receptors
    Receptor r1;
    r1.setPosition({0.0, 0.0, 0.0});
    r1.setCharge(-1.5f);
    r1.setRadius(0.4f);
    r1.setType("LPS");
    m_receptors.push_back(r1);
    
    Receptor r2;
    r2.setPosition({1.0f, 0.8f, 0.0f});
    r2.setCharge(-0.8f);
    r2.setRadius(0.35f);
    r2.setType("OmpF");
    m_receptors.push_back(r2);
}

void BacterialStrain::updateQuorumSensing(double dt) {
    // Update quorum sensing with current population density
    m_quorum_sensing.setPopulationDensity(m_population_density);
    m_quorum_sensing.update(dt);
    
    // Update growth rate based on quorum
    updateGrowthRateFromQuorum();
}

void BacterialStrain::updatePopulation(double dt) {
    // Logistic growth: dP/dt = r * P * (1 - P/K)
    double dP_dt = m_growth_rate * m_population_density * 
                   (1.0 - m_population_density / m_max_population);
    
    // Death rate (constant)
    dP_dt -= m_death_rate * m_population_density;
    
    // Update population
    m_population_density += dP_dt * dt;
    
    // Clamp
    m_population_density = std::max(0.0, std::min(m_population_density, m_max_population));
}

void BacterialStrain::updateGrowthRateFromQuorum() {
    // Quorum sensing affects growth rate (biofilm formation, etc.)
    double ahl = m_quorum_sensing.getAHL();
    double quorum_factor = std::tanh(ahl / 0.5);
    
    // Base growth rate + quorum enhancement
    double base_growth = 0.1;
    double max_enhancement = 0.05;
    m_growth_rate = base_growth + max_enhancement * quorum_factor;
}

void BacterialStrain::mutateReceptors(double mutation_rate, std::mt19937& rng) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    for (auto& receptor : m_receptors) {
        if (dist(rng) < mutation_rate) {
            mutateReceptor(receptor, rng);
        }
    }
}

void BacterialStrain::mutateReceptor(Receptor& receptor, std::mt19937& rng) {
    std::uniform_real_distribution<double> dist(-0.5, 0.5);
    
    // Mutate charge
    float new_charge = receptor.getCharge() + static_cast<float>(dist(rng));
    receptor.setCharge(new_charge);
    
    // Mutate position slightly
    core::Point3D pos = receptor.getPosition();
    pos.x += static_cast<float>(dist(rng)) * 0.2f;
    pos.y += static_cast<float>(dist(rng)) * 0.2f;
    pos.z += static_cast<float>(dist(rng)) * 0.2f;
    receptor.setPosition(pos);
    
    // Mutate radius
    float new_radius = receptor.getRadius() + static_cast<float>(dist(rng)) * 0.05f;
    receptor.setRadius(std::max(0.1f, new_radius));
}

double BacterialStrain::computePhageBindingScore(const Genome& phage) const {
    // Simplified binding score based on charge complementarity
    // Phase 3 will implement full electrostatics
    double score = 0.0;
    auto aa_sequence = phage.translateTailFiber();
    
    // For each amino acid in the phage tail
    for (size_t i = 0; i < aa_sequence.size() && i < 20; ++i) {
        auto aa = aa_sequence[i];
        auto props = AminoAcidPropertiesManager::instance().getProperties(aa);
        double phage_charge = props.net_charge_at_ph7;
        
        // For each bacterial receptor
        for (const auto& receptor : m_receptors) {
            // Opposite charges attract
            double interaction = -phage_charge * receptor.getCharge();
            
            // Distance factor (simplified)
            double distance = 1.0 + i * 0.1;  // Rough estimate
            interaction /= distance;
            
            score += interaction;
        }
    }
    
    return score;
}

bool BacterialStrain::infectWithPhage(const Genome& phage, double infectivity) {
    // Compute binding score
    double binding_score = computePhageBindingScore(phage);
    
    // Normalize score to determine infection probability
    double infection_probability = std::tanh(binding_score / 10.0) * infectivity;
    
    // Infection reduces population
    double population_loss = m_population_density * infection_probability * 0.2;
    m_population_density = std::max(0.0, m_population_density - population_loss);
    
    // Return true if infection was successful
    return infection_probability > 0.5;
}

double BacterialStrain::getBindingResistance() const {
    // Compute resistance based on receptor charges
    double total_charge = 0.0;
    for (const auto& receptor : m_receptors) {
        total_charge += receptor.getCharge();
    }
    return std::abs(total_charge) / m_receptors.size();
}

double BacterialStrain::logisticGrowth(double population, double max_pop, double rate) {
    return rate * population * (1.0 - population / max_pop);
}

} // namespace phageforge::biology
