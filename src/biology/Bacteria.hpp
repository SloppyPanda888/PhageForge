#pragma once

#include "../core/Types.hpp"
#include "Receptor.hpp"
#include "QuorumSensing.hpp"
#include <vector>
#include <string>
#include <random>

namespace phageforge::biology {

/**
 * @brief Bacterial strain with quorum sensing and phage interaction
 * 
 * Phase 2: Full implementation with:
 * - Population growth (logistic equation)
 * - Quorum sensing integration
 * - Receptor mutation under phage pressure
 * - Phage infection simulation
 */
class BacterialStrain {
public:
    BacterialStrain();
    ~BacterialStrain() = default;
    
    // --- Basic Properties ---
    void setName(const std::string& name) { m_name = name; }
    const std::string& getName() const { return m_name; }
    
    void setPopulationDensity(double density) { m_population_density = density; }
    double getPopulationDensity() const { return m_population_density; }
    
    void setMaxPopulation(double max_pop) { m_max_population = max_pop; }
    double getMaxPopulation() const { return m_max_population; }
    
    void setGrowthRate(double rate) { m_growth_rate = rate; }
    double getGrowthRate() const { return m_growth_rate; }
    
    // --- Receptors ---
    void addReceptor(const Receptor& receptor) { m_receptors.push_back(receptor); }
    const std::vector<Receptor>& getReceptors() const { return m_receptors; }
    std::vector<Receptor>& getReceptorsMutable() { return m_receptors; }
    void clearReceptors() { m_receptors.clear(); }
    
    // --- Quorum Sensing ---
    QuorumSensingSystem& getQuorumSensing() { return m_quorum_sensing; }
    const QuorumSensingSystem& getQuorumSensing() const { return m_quorum_sensing; }
    void updateQuorumSensing(double dt);
    bool isQuorumReached() const { return m_quorum_sensing.isQuorumReached(); }
    
    // --- Population Dynamics ---
    void updatePopulation(double dt);
    void updateGrowthRateFromQuorum();
    
    // --- Mutation ---
    void mutateReceptors(double mutation_rate, std::mt19937& rng);
    void mutateReceptor(Receptor& receptor, std::mt19937& rng);
    
    // --- Phage Interaction ---
    double computePhageBindingScore(const Genome& phage) const;
    bool infectWithPhage(const Genome& phage, double infectivity);
    
    // --- Stats ---
    double getBindingResistance() const;
    double getInfectionRate() const { return m_infection_rate; }
    void setInfectionRate(double rate) { m_infection_rate = rate; }
    
private:
    // Basic properties
    std::string m_name = "E. coli";
    double m_population_density = 0.1;   // OD600
    double m_max_population = 10.0;       // OD600
    double m_growth_rate = 0.1;           // per hour
    double m_death_rate = 0.01;           // per hour
    
    // Receptors
    std::vector<Receptor> m_receptors;
    
    // Quorum sensing
    QuorumSensingSystem m_quorum_sensing;
    
    // Infection tracking
    double m_infection_rate = 0.0;        // 0.0 = resistant, 1.0 = fully susceptible
    
    // Helper for logistic growth
    double logisticGrowth(double population, double max_pop, double rate);
};

} // namespace phageforge::biology
