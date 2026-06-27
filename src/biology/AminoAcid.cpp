#include "AminoAcid.hpp"
#include "../core/Exceptions.hpp"
#include <fmt/format.h>
#include <fstream>
#include <algorithm>

namespace phageforge::biology {

AminoAcidPropertiesManager& AminoAcidPropertiesManager::instance() {
    static AminoAcidPropertiesManager manager;
    return manager;
}

void AminoAcidPropertiesManager::loadFromTOML(const std::string& filepath) {
    try {
        auto config = toml::parse_file(filepath);
        
        // Get the amino_acids table
        if (auto table = config["amino_acids"].as_table()) {
            parseTOMLTable(*table);
            m_loaded = true;
        } else {
            throw core::ConfigurationException(
                fmt::format("Missing 'amino_acids' table in {}", filepath)
            );
        }
    } catch (const toml::parse_error& e) {
        throw core::ConfigurationException(
            fmt::format("Failed to parse TOML file {}: {}", filepath, e.what())
        );
    }
}

void AminoAcidPropertiesManager::parseTOMLTable(const toml::table& table) {
    // Expected format in TOML:
    // [amino_acids.ALA]
    // one_letter = "A"
    // three_letter = "ALA"
    // full_name = "Alanine"
    // net_charge_at_ph7 = 0.0
    // isoelectric_point = 6.0
    // hydrophobicity = 1.8
    // van_der_waals_radius = 0.25
    // molecular_weight = 89.09
    // helix_propensity = 1.0
    // sheet_propensity = 0.9
    // turn_propensity = 0.5
    
    for (const auto& [key, value] : table) {
        // key should be e.g., "ALA", "ARG", etc.
        auto aa_opt = core::stringToAminoAcid(key.str());
        if (!aa_opt) {
            // Skip unknown keys
            continue;
        }
        
        auto aa = *aa_opt;
        auto& props = m_properties[aa];
        props.code = aa;
        
        // Parse the sub-table
        if (auto subtable = value.as_table()) {
            // Required fields
            props.one_letter = (*subtable)["one_letter"].value_or<std::string>("");
            props.three_letter = (*subtable)["three_letter"].value_or<std::string>("");
            props.full_name = (*subtable)["full_name"].value_or<std::string>("");
            
            // Numeric fields with defaults
            props.net_charge_at_ph7 = (*subtable)["net_charge_at_ph7"].value_or(0.0f);
            props.isoelectric_point = (*subtable)["isoelectric_point"].value_or(6.0f);
            props.hydrophobicity = (*subtable)["hydrophobicity"].value_or(0.0f);
            props.van_der_waals_radius = (*subtable)["van_der_waals_radius"].value_or(0.3f);
            props.molecular_weight = (*subtable)["molecular_weight"].value_or(100.0f);
            props.helix_propensity = (*subtable)["helix_propensity"].value_or(0.5f);
            props.sheet_propensity = (*subtable)["sheet_propensity"].value_or(0.5f);
            props.turn_propensity = (*subtable)["turn_propensity"].value_or(0.5f);
        }
    }
}

const core::AminoAcidProperties& AminoAcidPropertiesManager::getProperties(core::AminoAcidCode aa) const {
    if (!m_loaded) {
        throw core::RuntimeException("Amino acid properties not loaded. Call loadFromTOML first.");
    }
    
    auto it = m_properties.find(aa);
    if (it == m_properties.end()) {
        throw core::InvalidArgumentException(
            fmt::format("Properties for amino acid {} not found", 
                       core::aminoAcidToString(aa))
        );
    }
    return it->second;
}

float AminoAcidPropertiesManager::getChargeAtPH(core::AminoAcidCode aa, float ph) const {
    // Simplified: for now, just return the pH 7 charge
    // Future: implement Henderson-Hasselbalch with side chain pKa values
    return getNetCharge(aa);
}

} // namespace phageforge::biology
