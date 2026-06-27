#pragma once

#include <cstdint>
#include <array>
#include <string>
#include <vector>
#include <optional>

namespace phageforge::core {

// --- Fundamental Type Aliases ---
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using f32 = float;
using f64 = double;

// --- Biological Constants ---
static constexpr u32 MAX_CODONS_PER_GENE = 512;   // Max 512 codons (170 amino acids)
static constexpr u32 NUM_BASES_PER_CODON = 3;
static constexpr u32 MAX_PHAGE_TAIL_LENGTH = 20;  // Only the tip matters for binding

// --- DNA Base Representation ---
enum class DNABase : u8 {
    ADENINE   = 0,
    CYTOSINE  = 1,
    GUANINE   = 2,
    THYMINE   = 3,
    INVALID   = 0xFF
};

// Convert base to char for display
[[nodiscard]] constexpr char baseToChar(DNABase base) noexcept {
    switch(base) {
        case DNABase::ADENINE:  return 'A';
        case DNABase::CYTOSINE: return 'C';
        case DNABase::GUANINE:  return 'G';
        case DNABase::THYMINE:  return 'T';
        default:                return '?';
    }
}

// Convert char to base (case insensitive)
[[nodiscard]] constexpr DNABase charToBase(char c) noexcept {
    switch(c) {
        case 'A': case 'a': return DNABase::ADENINE;
        case 'C': case 'c': return DNABase::CYTOSINE;
        case 'G': case 'g': return DNABase::GUANINE;
        case 'T': case 't': return DNABase::THYMINE;
        default:            return DNABase::INVALID;
    }
}

// --- Standard Amino Acid 3-Letter Codes ---
enum class AminoAcidCode : u8 {
    ALA, ARG, ASN, ASP, CYS, GLN, GLU, GLY, HIS, ILE,
    LEU, LYS, MET, PHE, PRO, SER, THR, TRP, TYR, VAL,
    STOP  // Termination codon
};

// Convert amino acid to human-readable string
[[nodiscard]] std::string aminoAcidToString(AminoAcidCode aa) noexcept;

// Convert 3-letter code to enum (e.g., "ALA" -> AminoAcidCode::ALA)
[[nodiscard]] std::optional<AminoAcidCode> stringToAminoAcid(const std::string& three_letter) noexcept;

// --- Physical Properties of Amino Acids (will be loaded from TOML) ---
struct AminoAcidProperties {
    AminoAcidCode code;
    std::string one_letter;           // e.g., "A" for Alanine
    std::string three_letter;         // e.g., "ALA"
    std::string full_name;            // e.g., "Alanine"
    
    // Charged properties (in units of elementary charge, e)
    float net_charge_at_ph7;          // -1.0 for Asp/Glu, +1.0 for Lys/Arg, 0 for neutral
    float isoelectric_point;          // pI for pH-dependent charging (future)
    
    // Hydrophobicity (Kyte-Doolittle scale, typically -4.5 to +4.5)
    float hydrophobicity;             // Positive = hydrophobic, Negative = hydrophilic
    
    // Physical dimensions (in nanometers)
    float van_der_waals_radius;       // Radius of the amino acid side chain
    float molecular_weight;           // In Daltons (g/mol)
    
    // Flexiblity / secondary structure propensity
    float helix_propensity;           // 0.0 to 1.0
    float sheet_propensity;           // 0.0 to 1.0
    float turn_propensity;            // 0.0 to 1.0
};

// --- Position in 3D space (using Eigen in the physics layer) ---
// We define a lightweight alias here to avoid coupling core to Eigen
struct Point3D {
    float x, y, z;
    
    // Basic operations (no Eigen dependency)
    [[nodiscard]] Point3D operator+(const Point3D& other) const noexcept;
    [[nodiscard]] Point3D operator-(const Point3D& other) const noexcept;
    [[nodiscard]] Point3D operator*(float scalar) const noexcept;
    [[nodiscard]] float distanceTo(const Point3D& other) const noexcept;
    [[nodiscard]] float squaredDistanceTo(const Point3D& other) const noexcept;
};

} // namespace phageforge::core
