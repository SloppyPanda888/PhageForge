#include "Electrostatics.hpp"

namespace phageforge::physics {

// Static member initialization
float ElectrostaticsSolver::m_ionic_strength = core::constants::IONIC_STRENGTH;
float ElectrostaticsSolver::m_temperature = core::constants::TEMPERATURE;

// All functions are implemented inline in the header
// This .cpp file exists to provide a translation unit for the library

} // namespace phageforge::physics
