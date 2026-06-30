#pragma once

#include "../core/Types.hpp"
#include <boost/numeric/odeint.hpp>
#include <array>
#include <vector>

namespace phageforge::biology {

/**
 * @brief Quorum Sensing System for Bacterial Communication
 * 
 * Models the LuxI/LuxR/AHL system using ODEs:
 * - d(LuxI)/dt = production - degradation
 * - d(LuxR)/dt = production - degradation
 * - d(AHL)/dt = LuxI * LuxR - degradation
 */
class QuorumSensingSystem {
public:
    // State vector: [LuxI, LuxR, AHL]
    using State = std::array<double, 3>;
    
    QuorumSensingSystem();
    ~QuorumSensingSystem() = default;
    
    // Update the system (ODE integration)
    void update(double dt);
    
    // Getters for current concentrations
    double getLuxI() const { return m_state[0]; }
    double getLuxR() const { return m_state[1]; }
    double getAHL() const { return m_state[2]; }
    double getPopulationFactor() const { return m_population_factor; }
    
    // Setters for parameters
    void setPopulationDensity(double density) { m_population_density = density; }
    void setQuorumThreshold(double threshold) { m_quorum_threshold = threshold; }
    void setProductionRates(double luxI_prod, double luxR_prod);
    
    // Check if quorum is reached
    bool isQuorumReached() const { return m_state[2] > m_quorum_threshold; }
    
    // Reset the system
    void reset();
    
    // Get the state for visualization
    const State& getState() const { return m_state; }
    
private:
    State m_state;                    // [LuxI, LuxR, AHL]
    double m_population_density;       // Bacterial population (OD600)
    double m_population_factor;        // Population influence on production
    double m_quorum_threshold;         // AHL threshold for quorum
    
    // Parameters (Phase 2)
    double m_luxI_production;          // α: LuxI production rate
    double m_luxI_degradation;         // β: LuxI degradation rate
    double m_luxR_production;          // γ: LuxR production rate
    double m_luxR_degradation;         // δ: LuxR degradation rate
    double m_ahl_synthase_factor;      // ε: AHL synthesis from LuxI*LuxR
    double m_ahl_degradation;          // ζ: AHL degradation rate
    
    // ODE stepper
    boost::numeric::odeint::runge_kutta_dopri5<State> m_stepper;
    
    // Helper to compute derivatives
    void computeDerivatives(const State& x, State& dxdt, double t);
};

} // namespace phageforge::biology
