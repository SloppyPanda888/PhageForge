#pragma once

#include "Types.hpp"

namespace phageforge::core::constants {

// --- Physical Constants (SI units, but we'll use nm and elementary charge for simulation) ---

// Elementary charge (Coulombs)
static constexpr f64 ELEMENTARY_CHARGE = 1.602176634e-19; 

// Coulomb's constant (N m^2 / C^2)
static constexpr f64 COULOMB_CONSTANT = 8.9875517923e9;

// Vacuum permittivity (F/m)
static constexpr f64 EPSILON_0 = 8.8541878128e-12;

// Boltzmann constant (J/K)
static constexpr f64 BOLTZMANN_CONSTANT = 1.380649e-23;

// Avogadro's number (mol^-1)
static constexpr f64 AVOGADRO = 6.02214076e23;

// --- Simulation Constants (in game units: nanometers, elementary charge, Kelvin) ---

// Temperature (K) - standard physiological
static constexpr f32 TEMPERATURE = 310.0f;  // 37°C

// Relative permittivity of water at 310K
static constexpr f32 DIELECTRIC_CONSTANT_WATER = 78.4f;

// Debye-Hückel parameters:
// Ionic strength (M) of typical bacterial growth medium (LB broth)
static constexpr f32 IONIC_STRENGTH = 0.15f;  // 150 mM NaCl equivalent

// Inverse Debye length (kappa) in nm^-1
// Calculated at runtime from ionic strength, but we precompute for speed
static constexpr f32 DEBYE_LENGTH_INVERSE = 0.329f;  // For 150 mM monovalent salt at 310K

// --- Biological Constants ---

// Number of amino acids in a typical phage tail fiber (just the binding domain)
static constexpr u32 TAIL_FIBER_BINDING_DOMAIN_LENGTH = 85;

// Average distance between adjacent amino acids in a protein (nm)
static constexpr f32 AVERAGE_CA_CA_DISTANCE = 0.38f;  // 3.8 Å

// Typical binding affinity threshold for effective infection (arbitrary units)
static constexpr f32 INFECTION_THRESHOLD = -15.0f;  // Negative = strong binding

} // namespace phageforge::core::constants
