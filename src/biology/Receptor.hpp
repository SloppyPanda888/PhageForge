#pragma once

#include "../core/Types.hpp"
#include <string>

namespace phageforge::biology {

/**
 * @brief Bacterial surface receptor for phage binding
 */
class Receptor {
public:
    Receptor() = default;
    ~Receptor() = default;
    
    // Position on bacterial surface (nm)
    void setPosition(const core::Point3D& pos) { m_position = pos; }
    core::Point3D getPosition() const { return m_position; }
    
    void setX(float x) { m_position.x = x; }
    void setY(float y) { m_position.y = y; }
    void setZ(float z) { m_position.z = z; }
    
    // Charge properties (in elementary charge units)
    void setCharge(float charge) { m_charge = charge; }
    float getCharge() const { return m_charge; }
    
    // Size of the receptor (nm)
    void setRadius(float radius) { m_radius = radius; }
    float getRadius() const { return m_radius; }
    
    // Receptor type/identity
    void setType(const std::string& type) { m_type = type; }
    const std::string& getType() const { return m_type; }
    
    // Binding affinity for a specific phage (Phase 2 implementation)
    void setBindingAffinity(float affinity) { m_binding_affinity = affinity; }
    float getBindingAffinity() const { return m_binding_affinity; }
    
    // Mutate the receptor (Phase 2 implementation)
    void mutate() { 
        // Phase 2: Change charge, position, or type
        // in response to phage pressure
    }
    
private:
    core::Point3D m_position{0.0f, 0.0f, 0.0f};  // Position in nm
    float m_charge = 0.0f;                        // Charge in e units
    float m_radius = 0.3f;                       // Radius in nm
    float m_binding_affinity = 0.0f;             // Affinity score
    std::string m_type = "unknown";              // Receptor type
};

} // namespace phageforge::biology
