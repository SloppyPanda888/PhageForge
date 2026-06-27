#pragma once

#include "../core/Types.hpp"
#include <unordered_map>
#include <string>

namespace phageforge::biology {

// Manager for amino acid physical properties
class AminoAcidPropertiesManager {
public:
    // Singleton access
    static AminoAcidPropertiesManager& instance();
    
    // Load properties from file (stub for Phase 1 - uses hardcoded values)
    void loadFromTOML(const std::string& filepath);
    
    // Get properties for a specific amino acid
    [[nodiscard]] const core::AminoAcidProperties& getProperties(core::AminoAcidCode aa) const;
    
    // Get charge at a specific pH (future: implement Henderson-Hasselbalch)
    [[nodiscard]] float getChargeAtPH(core::AminoAcidCode aa, float ph) const;
    
    // Getters for quick access (used heavily in physics engine)
    [[nodiscard]] inline float getNetCharge(core::AminoAcidCode aa) const {
        return getProperties(aa).net_charge_at_ph7;
    }
    
    [[nodiscard]] inline float getHydrophobicity(core::AminoAcidCode aa) const {
        return getProperties(aa).hydrophobicity;
    }
    
    [[nodiscard]] inline float getVdWRadius(core::AminoAcidCode aa) const {
        return getProperties(aa).van_der_waals_radius;
    }
    
private:
    AminoAcidPropertiesManager() = default;  // Private constructor
    
    std::unordered_map<core::AminoAcidCode, core::AminoAcidProperties> m_properties;
    bool m_loaded = false;
    
    // Helper to set properties (used by manual loading)
    void setProperty(core::AminoAcidCode code, 
                     const std::string& one_letter,
                     const std::string& three_letter,
                     const std::string& full_name,
                     float charge,
                     float hydro,
                     float radius,
                     float weight);
};

} // namespace phageforge::biology
