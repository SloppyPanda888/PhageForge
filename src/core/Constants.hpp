#pragma once

#include "Types.hpp"

namespace phageforge::core::constants {

// --- Physical Constants ---
static constexpr f64 ELEMENTARY_CHARGE = 1.602176634e-19;      // C
static constexpr f64 COULOMB_CONSTANT = 8.9875517923e9;        // N·m²/C²
static constexpr f64 EPSILON_0 = 8.8541878128e-12;             // F/m
static constexpr f64 BOLTZMANN_CONSTANT = 1.380649e-23;        // J/K
static constexpr f64 AVOGADRO = 6.02214076e23;                 // mol⁻¹
static constexpr f64 K_B = 1.380649e-23;                       // J/K
static constexpr f64 E_CHARGE = 1.602176634e-19;               // C

// --- Simulation Constants ---
static constexpr f32 TEMPERATURE = 310.0f;                     // 37°C (K)
static constexpr f32 DIELECTRIC_CONSTANT_WATER = 78.4f;        // at 310K
static constexpr f32 IONIC_STRENGTH = 0.15f;                   // M (150 mM NaCl)

// --- Debye-Hückel Constants ---
static constexpr f32 DEBYE_LENGTH_INVERSE = 0.329f;            // nm⁻¹ at 150mM, 310K
static constexpr f32 DEBYE_LENGTH = 1.0f / DEBYE_LENGTH_INVERSE; // nm

// --- Biological Constants ---
static constexpr u32 TAIL_FIBER_BINDING_DOMAIN_LENGTH = 85;
static constexpr f32 AVERAGE_CA_CA_DISTANCE = 0.38f;           // nm
static constexpr f32 INFECTION_THRESHOLD = -15.0f;             // kJ/mol

// --- Phase 3: Electrostatics ---
static constexpr f32 MIN_DISTANCE = 0.05f;                     // nm (avoid singularities)
static constexpr f32 MAX_INTERACTION_DISTANCE = 10.0f;         // nm (cutoff)

} // namespace phageforge::core::constants
