#pragma once

#include "../core/Types.hpp"
#include "Receptor.hpp"
#include <vector>
#include <string>

namespace phageforge::biology {

/**
 * @brief Represents a bacterial strain in the PhageForge game
 * 
 * For Phase 1: This is a stub. Phase 2 will implement:
 * - Population dynamics (growth curves, death rates)
 * - Quorum sensing integration
 * - Receptor mutation in response to phage pressure
 */
class BacterialStrain {
public:
    BacterialStrain() = default;
    ~BacterialStrain() = default;
    
    // Basic properties
    void setName(const std::string& name) { m_name = name; }
    const std::string& getName() const { return m_name; }
    
    // Receptors on the bacterial surface
    void addReceptor(const Receptor& receptor) { m_receptors.push_back(receptor); }
    const std::vector<Receptor>& getReceptors() const { return m_receptors; }
    std::vector<Receptor>& getReceptorsMutable() { return m_receptors; }
    void clearReceptors() { m_receptors.clear(); }
    
    // Population dynamics (Phase 2 implementation)
    void setPopulationDensity(float density) { m_population_density = density; }
    float getPopulationDensity() const { return m_population_density; }
    
    // Mutation (Phase 2 implementation)
    void mutateReceptors() { /* Phase 2 implementation */ }
    
    // Quorum sensing (Phase 2 implementation)
    void updateQuorumSensing(float dt) { /* Phase 2 implementation */ }
    float getAHLConcentration() const { return 0.0f; }
    
private:
    std::string m_name = "E. coli";
    std::vector<Receptor> m_receptors;
    float m_population_density = 1.0f;  // OD600 units
};

} // namespace phageforge::biology
