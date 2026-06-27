#include "AminoAcid.hpp"
#include "../core/Exceptions.hpp"
#include <iostream>

namespace phageforge::biology {

AminoAcidPropertiesManager& AminoAcidPropertiesManager::instance() {
    static AminoAcidPropertiesManager manager;
    return manager;
}

void AminoAcidPropertiesManager::setProperty(core::AminoAcidCode code, 
                                              const std::string& one_letter,
                                              const std::string& three_letter,
                                              const std::string& full_name,
                                              float charge,
                                              float hydro,
                                              float radius,
                                              float weight) {
    auto& props = m_properties[code];
    props.code = code;
    props.one_letter = one_letter;
    props.three_letter = three_letter;
    props.full_name = full_name;
    props.net_charge_at_ph7 = charge;
    props.isoelectric_point = 6.0f;
    props.hydrophobicity = hydro;
    props.van_der_waals_radius = radius;
    props.molecular_weight = weight;
    props.helix_propensity = 0.5f;
    props.sheet_propensity = 0.5f;
    props.turn_propensity = 0.5f;
}

void AminoAcidPropertiesManager::loadFromTOML(const std::string& /*filepath*/) {
    std::cout << "  Loading amino acid properties (hardcoded for Phase 1)..." << std::endl;
    
    setProperty(core::AminoAcidCode::ALA, "A", "ALA", "Alanine", 0.0f, 1.8f, 0.25f, 89.09f);
    setProperty(core::AminoAcidCode::ARG, "R", "ARG", "Arginine", 1.0f, -4.5f, 0.35f, 174.20f);
    setProperty(core::AminoAcidCode::ASN, "N", "ASN", "Asparagine", 0.0f, -3.5f, 0.28f, 132.12f);
    setProperty(core::AminoAcidCode::ASP, "D", "ASP", "Aspartic Acid", -1.0f, -3.5f, 0.28f, 133.10f);
    setProperty(core::AminoAcidCode::CYS, "C", "CYS", "Cysteine", 0.0f, 2.5f, 0.30f, 121.16f);
    setProperty(core::AminoAcidCode::GLN, "Q", "GLN", "Glutamine", 0.0f, -3.5f, 0.30f, 146.15f);
    setProperty(core::AminoAcidCode::GLU, "E", "GLU", "Glutamic Acid", -1.0f, -3.5f, 0.30f, 147.13f);
    setProperty(core::AminoAcidCode::GLY, "G", "GLY", "Glycine", 0.0f, -0.4f, 0.20f, 75.07f);
    setProperty(core::AminoAcidCode::HIS, "H", "HIS", "Histidine", 0.1f, -3.2f, 0.32f, 155.16f);
    setProperty(core::AminoAcidCode::ILE, "I", "ILE", "Isoleucine", 0.0f, 4.5f, 0.32f, 131.17f);
    setProperty(core::AminoAcidCode::LEU, "L", "LEU", "Leucine", 0.0f, 3.8f, 0.32f, 131.17f);
    setProperty(core::AminoAcidCode::LYS, "K", "LYS", "Lysine", 1.0f, -3.9f, 0.34f, 146.19f);
    setProperty(core::AminoAcidCode::MET, "M", "MET", "Methionine", 0.0f, 1.9f, 0.32f, 149.21f);
    setProperty(core::AminoAcidCode::PHE, "F", "PHE", "Phenylalanine", 0.0f, 2.8f, 0.35f, 165.19f);
    setProperty(core::AminoAcidCode::PRO, "P", "PRO", "Proline", 0.0f, -1.6f, 0.30f, 115.13f);
    setProperty(core::AminoAcidCode::SER, "S", "SER", "Serine", 0.0f, -0.8f, 0.26f, 105.09f);
    setProperty(core::AminoAcidCode::THR, "T", "THR", "Threonine", 0.0f, -0.7f, 0.28f, 119.12f);
    setProperty(core::AminoAcidCode::TRP, "W", "TRP", "Tryptophan", 0.0f, -0.9f, 0.38f, 204.23f);
    setProperty(core::AminoAcidCode::TYR, "Y", "TYR", "Tyrosine", 0.0f, -1.3f, 0.36f, 181.19f);
    setProperty(core::AminoAcidCode::VAL, "V", "VAL", "Valine", 0.0f, 4.2f, 0.30f, 117.15f);
    setProperty(core::AminoAcidCode::STOP, "*", "STP", "Stop Codon", 0.0f, 0.0f, 0.0f, 0.0f);
    
    m_loaded = true;
    std::cout << "  ✓ Successfully loaded all 20 amino acids (hardcoded)" << std::endl;
}

const core::AminoAcidProperties& AminoAcidPropertiesManager::getProperties(core::AminoAcidCode aa) const {
    if (!m_loaded) {
        throw core::RuntimeException("Amino acid properties not loaded. Call loadFromTOML first.");
    }
    
    auto it = m_properties.find(aa);
    if (it == m_properties.end()) {
        throw core::InvalidArgumentException(
            "Properties for amino acid not found"
        );
    }
    return it->second;
}

float AminoAcidPropertiesManager::getChargeAtPH(core::AminoAcidCode aa, float /*ph*/) const {
    return getNetCharge(aa);
}

} // namespace phageforge::biology
