#pragma once

#include "Types.hpp"

namespace phageforge::core::constants {

// --- Physical Constants ---
static constexpr f64 ELEMENTARY_CHARGE = 1.602176634e-19;
static constexpr f64 COULOMB_CONSTANT = 8.9875517923e9;
static constexpr f64 EPSILON_0 = 8.8541878128e-12;
static constexpr f64 BOLTZMANN_CONSTANT = 1.380649e-23;
static constexpr f64 AVOGADRO = 6.02214076e23;

// --- Simulation Constants ---
static constexpr f32 TEMPERATURE = 310.0f;      // 37°C
static constexpr f32 DIELECTRIC_CONSTANT_WATER = 78.4f;
static constexpr f32 IONIC_STRENGTH = 0.15f;    // 150 mM NaCl
static constexpr f32 DEBYE_LENGTH_INVERSE = 0.329f;  // nm^-1 at 150mM, 310K

// --- Biological Constants ---
static constexpr u32 TAIL_FIBER_BINDING_DOMAIN_LENGTH = 85;
static constexpr f32 AVERAGE_CA_CA_DISTANCE = 0.38f;  // nm
static constexpr f32 INFECTION_THRESHOLD = -15.0f;    // kJ/mol

} // namespace phageforge::core::constants
