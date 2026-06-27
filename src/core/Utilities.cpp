#include "Utilities.hpp"
#include <unordered_map>
#include <random>
#include <cmath>

namespace phageforge::core {

std::string aminoAcidToString(AminoAcidCode aa) noexcept {
    static const std::unordered_map<AminoAcidCode, std::string> mapping = {
        {AminoAcidCode::ALA, "ALA"},
        {AminoAcidCode::ARG, "ARG"},
        {AminoAcidCode::ASN, "ASN"},
        {AminoAcidCode::ASP, "ASP"},
        {AminoAcidCode::CYS, "CYS"},
        {AminoAcidCode::GLN, "GLN"},
        {AminoAcidCode::GLU, "GLU"},
        {AminoAcidCode::GLY, "GLY"},
        {AminoAcidCode::HIS, "HIS"},
        {AminoAcidCode::ILE, "ILE"},
        {AminoAcidCode::LEU, "LEU"},
        {AminoAcidCode::LYS, "LYS"},
        {AminoAcidCode::MET, "MET"},
        {AminoAcidCode::PHE, "PHE"},
        {AminoAcidCode::PRO, "PRO"},
        {AminoAcidCode::SER, "SER"},
        {AminoAcidCode::THR, "THR"},
        {AminoAcidCode::TRP, "TRP"},
        {AminoAcidCode::TYR, "TYR"},
        {AminoAcidCode::VAL, "VAL"},
        {AminoAcidCode::STOP, "STP"}
    };
    
    auto it = mapping.find(aa);
    if (it != mapping.end()) {
        return it->second;
    }
    return "UNK";
}

std::optional<AminoAcidCode> stringToAminoAcid(const std::string& three_letter) noexcept {
    static const std::unordered_map<std::string, AminoAcidCode> mapping = {
        {"ALA", AminoAcidCode::ALA},
        {"ARG", AminoAcidCode::ARG},
        {"ASN", AminoAcidCode::ASN},
        {"ASP", AminoAcidCode::ASP},
        {"CYS", AminoAcidCode::CYS},
        {"GLN", AminoAcidCode::GLN},
        {"GLU", AminoAcidCode::GLU},
        {"GLY", AminoAcidCode::GLY},
        {"HIS", AminoAcidCode::HIS},
        {"ILE", AminoAcidCode::ILE},
        {"LEU", AminoAcidCode::LEU},
        {"LYS", AminoAcidCode::LYS},
        {"MET", AminoAcidCode::MET},
        {"PHE", AminoAcidCode::PHE},
        {"PRO", AminoAcidCode::PRO},
        {"SER", AminoAcidCode::SER},
        {"THR", AminoAcidCode::THR},
        {"TRP", AminoAcidCode::TRP},
        {"TYR", AminoAcidCode::TYR},
        {"VAL", AminoAcidCode::VAL},
        {"STP", AminoAcidCode::STOP}
    };
    
    auto it = mapping.find(three_letter);
    if (it != mapping.end()) {
        return it->second;
    }
    return std::nullopt;
}

float randomFloat(float min_val, float max_val, std::mt19937& rng) {
    std::uniform_real_distribution<float> dist(min_val, max_val);
    return dist(rng);
}

// Point3D implementations
Point3D Point3D::operator+(const Point3D& other) const noexcept {
    return Point3D{x + other.x, y + other.y, z + other.z};
}

Point3D Point3D::operator-(const Point3D& other) const noexcept {
    return Point3D{x - other.x, y - other.y, z - other.z};
}

Point3D Point3D::operator*(float scalar) const noexcept {
    return Point3D{x * scalar, y * scalar, z * scalar};
}

float Point3D::distanceTo(const Point3D& other) const noexcept {
    float dx = x - other.x;
    float dy = y - other.y;
    float dz = z - other.z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

float Point3D::squaredDistanceTo(const Point3D& other) const noexcept {
    float dx = x - other.x;
    float dy = y - other.y;
    float dz = z - other.z;
    return dx*dx + dy*dy + dz*dz;
}

} // namespace phageforge::core
