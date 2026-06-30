#include "QuorumSensing.hpp"
#include <iostream>
#include <cmath>

namespace phageforge::biology {

QuorumSensingSystem::QuorumSensingSystem()
    : m_state{0.0, 0.0, 0.0},
      m_population_density(0.0),
      m_population_factor(0.0),
      m_quorum_threshold(0.8),
      m_luxI_production(0.5),
      m_luxI_degradation(0.1),
      m_luxR_production(0.2),
      m_luxR_degradation(0.05),
      m_ahl_synthase_factor(0.8),
      m_ahl_degradation(0.15) {
    reset();
}

void QuorumSensingSystem::computeDerivatives(const State& x, State& dxdt, double /*t*/) {
    // Extract state variables
    double LuxI = x[0];
    double LuxR = x[1];
    double AHL = x[2];
    
    // Population influence (density-dependent)
    double pop_factor = std::tanh(m_population_density / 2.0);  // Saturating
    m_population_factor = pop_factor;
    
    // ODEs:
    // d(LuxI)/dt = production * pop_factor - degradation * LuxI
    dxdt[0] = m_luxI_production * pop_factor - m_luxI_degradation * LuxI;
    
    // d(LuxR)/dt = production * pop_factor - degradation * LuxR
    dxdt[1] = m_luxR_production * pop_factor - m_luxR_degradation * LuxR;
    
    // d(AHL)/dt = synthase_factor * LuxI * LuxR - degradation * AHL
    dxdt[2] = m_ahl_synthase_factor * LuxI * LuxR - m_ahl_degradation * AHL;
    
    // Clamp to non-negative
    dxdt[0] = std::max(dxdt[0], 0.0);
    dxdt[1] = std::max(dxdt[1], 0.0);
    dxdt[2] = std::max(dxdt[2], 0.0);
}

void QuorumSensingSystem::update(double dt) {
    // Integrate the ODE system
    m_stepper.do_step(
        [this](const State& x, State& dxdt, double t) {
            computeDerivatives(x, dxdt, t);
        },
        m_state, 0.0, dt
    );
    
    // Clamp to non-negative (safety)
    m_state[0] = std::max(m_state[0], 0.0);
    m_state[1] = std::max(m_state[1], 0.0);
    m_state[2] = std::max(m_state[2], 0.0);
}

void QuorumSensingSystem::setProductionRates(double luxI_prod, double luxR_prod) {
    m_luxI_production = luxI_prod;
    m_luxR_production = luxR_prod;
}

void QuorumSensingSystem::reset() {
    m_state[0] = 0.0;   // LuxI
    m_state[1] = 0.0;   // LuxR
    m_state[2] = 0.0;   // AHL
    m_population_factor = 0.0;
}

} // namespace phageforge::biology
