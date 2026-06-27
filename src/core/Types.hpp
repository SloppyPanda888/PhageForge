#pragma once

#include <cstdint>
#include <array>
#include <string>
#include <vector>
#include <optional>
#include <random>
#include <cmath>

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
static constexpr u32 MAX_CODONS_PER_GENE = 512;
static constexpr u32 NUM_BASES_PER_CODON = 3;
static constexpr u32 MAX_PHAGE_TAIL_LENGTH = 20;

// --- DNA Base Representation ---
enum class DNABase : u8 {
    ADENINE   = 0,
    CYTOSINE  = 1,
    GUANINE   = 2,
    THYMINE   = 3,
    INVALID   = 0xFF
};

[[nodiscard]] constexpr char baseToChar(DNABase base) noexcept {
    switch(base) {
        case DNABase::ADENINE:  return 'A';
        case DNABase::CYTOSINE: return 'C';
        case DNABase::GUANINE:  return 'G';
        case DNABase::THYMINE:  return 'T';
        default:                return '?';
    }
}

[[nodiscard]] constexpr DNABase charToBase(char c) noexcept {
    switch(c) {
        case 'A': case 'a': return DNABase::ADENINE;
        case 'C': case 'c': return DNABase::CYTOSINE;
        case 'G': case 'g': return DNABase::GUANINE;
        case 'T': case 't': return DNABase::THYMINE;
        default:            return DNABase::INVALID;
    }
}

// --- Standard Amino Acid Codes ---
enum class AminoAcidCode : u8 {
    ALA, ARG, ASN, ASP, CYS, GLN, GLU, GLY, HIS, ILE,
    LEU, LYS, MET, PHE, PRO, SER, THR, TRP, TYR, VAL,
    STOP
};

// --- Physical Properties of Amino Acids ---
struct AminoAcidProperties {
    AminoAcidCode code;
    std::string one_letter;
    std::string three_letter;
    std::string full_name;
    
    float net_charge_at_ph7;
    float isoelectric_point;
    float hydrophobicity;
    float van_der_waals_radius;
    float molecular_weight;
    float helix_propensity;
    float sheet_propensity;
    float turn_propensity;
};

// --- Position in 3D Space ---
struct Point3D {
    float x, y, z;
    
    [[nodiscard]] Point3D operator+(const Point3D& other) const noexcept;
    [[nodiscard]] Point3D operator-(const Point3D& other) const noexcept;
    [[nodiscard]] Point3D operator*(float scalar) const noexcept;
    [[nodiscard]] float distanceTo(const Point3D& other) const noexcept;
    [[nodiscard]] float squaredDistanceTo(const Point3D& other) const noexcept;
};

} // namespace phageforge::core
