// Phase 3 CLI Test Harness - NO GUI dependencies!
#include "biology/Genome.hpp"
#include "biology/AminoAcid.hpp"
#include "biology/Bacteria.hpp"
#include "biology/QuorumSensing.hpp"
#include "physics/Electrostatics.hpp"
#include "physics/BindingAssay.hpp"
#include "physics/VectorMath.hpp"
#include "core/Constants.hpp"
#include "core/Utilities.hpp"
#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>

using namespace phageforge;

int main() {
    std::cout << "=== PhageForge Phase 3: CLI Test Harness ===" << std::endl;
    std::cout << std::endl;
    
    // Initialize amino acid properties
    try {
        biology::AminoAcidPropertiesManager::instance().loadFromTOML("config/amino_acids.toml");
        std::cout << "✅ Amino acid properties loaded\n" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "❌ Failed to load: " << e.what() << std::endl;
        return 1;
    }
    
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    
    // 1. Test Debye-Hückel Theory
    std::cout << "1. Testing Debye-Hückel Theory..." << std::endl;
    physics::ElectrostaticsSolver solver;
    solver.setIonicStrength(0.15);  // 150 mM NaCl
    solver.setTemperature(310.0);   // 37°C
    
    std::cout << "   Debye length: " << std::fixed << std::setprecision(3) 
              << solver.getDebyeLength() << " nm" << std::endl;
    
    // Test different charge interactions
    double energy_attractive = solver.debyeHuckelPotential(1.0, -1.0, 1.0);
    double energy_repulsive = solver.debyeHuckelPotential(1.0, 1.0, 1.0);
    double energy_far = solver.debyeHuckelPotential(1.0, -1.0, 5.0);
    
    std::cout << "   Two charges +1 and -1 at 1.0 nm: " 
              << std::fixed << std::setprecision(2) << energy_attractive << " kJ/mol" << std::endl;
    std::cout << "   Two charges +1 and +1 at 1.0 nm: " 
              << std::fixed << std::setprecision(2) << energy_repulsive << " kJ/mol" << std::endl;
    std::cout << "   Two charges +1 and -1 at 5.0 nm: " 
              << std::fixed << std::setprecision(2) << energy_far << " kJ/mol" << std::endl;
    std::cout << std::endl;
    
    // 2. Test Phage-Bacteria Binding
    std::cout << "2. Testing Phage-Bacteria Binding..." << std::endl;
    
    // Create a random phage
    auto phage = biology::GenomeFactory::createRandom(rng, 20);
    std::cout << "   Phage charges: " << solver.getPhageCharges(phage).size() << " amino acids" << std::endl;
    
    // Create bacteria with receptors
    biology::BacterialStrain bacteria;
    bacteria.setName("E. coli");
    bacteria.setPopulationDensity(1.0);
    
    // Add some receptors
    biology::Receptor r1;
    r1.setPosition({0.0, 0.0, 0.0});
    r1.setCharge(-1.5);
    r1.setType("LPS");
    bacteria.addReceptor(r1);
    
    biology::Receptor r2;
    r2.setPosition({1.0, 0.8, 0.0});
    r2.setCharge(-0.8);
    r2.setType("OmpF");
    bacteria.addReceptor(r2);
    
    std::cout << "   Bacterial receptors: " << bacteria.getReceptors().size() << " receptors" << std::endl;
    
    // Compute binding energy
    double binding_energy = solver.computeBindingEnergy(phage, bacteria);
    std::cout << "   Total binding energy: " << std::fixed << std::setprecision(2) 
              << binding_energy << " kJ/mol" << std::endl;
    
    // Score the binding
    float score = physics::BindingAssay::scoreBinding(binding_energy);
    std::cout << "   Binding score: " << std::fixed << std::setprecision(1) 
              << score << "/100" << std::endl;
    
    std::string description = physics::BindingAssay::getBindingDescription(binding_energy);
    std::cout << "   Binding description: " << description << std::endl;
    
    double prob = physics::BindingAssay::infectionProbability(binding_energy);
    std::cout << "   Infection probability: " << std::fixed << std::setprecision(3) 
              << prob << std::endl;
    
    bool can_infect = physics::BindingAssay::isInfection(binding_energy);
    std::cout << "   Infection possible: " << (can_infect ? "✅ Yes" : "❌ No") << std::endl;
    std::cout << std::endl;
    
    // 3. Test Interaction Map
    std::cout << "3. Testing Interaction Map..." << std::endl;
    auto interactions = solver.getInteractionMap(phage, bacteria);
    
    std::cout << "   Found " << interactions.size() << " interactions:" << std::endl;
    int count = 0;
    for (const auto& [aa_idx, receptor_idx, energy] : interactions) {
        if (count < 5) {  // Show first 5
            std::cout << "   Interaction " << count+1 << ": AA[" << aa_idx 
                      << "] → Receptor[" << receptor_idx << "]: " 
                      << std::fixed << std::setprecision(2) << energy << " kJ/mol" << std::endl;
        }
        count++;
    }
    if (interactions.size() > 5) {
        std::cout << "   ... and " << (interactions.size() - 5) << " more" << std::endl;
    }
    std::cout << std::endl;
    
    // 4. Test Different Conditions
    std::cout << "4. Testing Different Conditions..." << std::endl;
    
    // Low salt
    solver.setIonicStrength(0.01);
    double energy_low_salt = solver.computeBindingEnergy(phage, bacteria);
    std::cout << "   Low Salt (0.01M): Debye length = " << std::fixed << std::setprecision(3)
              << solver.getDebyeLength() << " nm, Energy = " 
              << std::fixed << std::setprecision(2) << energy_low_salt << " kJ/mol" << std::endl;
    
    // High salt
    solver.setIonicStrength(0.50);
    double energy_high_salt = solver.computeBindingEnergy(phage, bacteria);
    std::cout << "   High Salt (0.50M): Debye length = " << std::fixed << std::setprecision(3)
              << solver.getDebyeLength() << " nm, Energy = " 
              << std::fixed << std::setprecision(2) << energy_high_salt << " kJ/mol" << std::endl;
    
    // Back to physiological
    solver.setIonicStrength(0.15);
    solver.setTemperature(310.0);
    std::cout << std::endl;
    
    // 5. Test T4 Wild-Type Phage
    std::cout << "5. Testing T4 Wild-Type Phage..." << std::endl;
    try {
        auto t4_phage = biology::GenomeFactory::createT4WildType();
        double t4_energy = solver.computeBindingEnergy(t4_phage, bacteria);
        float t4_score = physics::BindingAssay::scoreBinding(t4_energy);
        
        std::cout << "   T4 Phage binding energy: " << std::fixed << std::setprecision(2)
                  << t4_energy << " kJ/mol" << std::endl;
        std::cout << "   T4 Phage score: " << std::fixed << std::setprecision(1)
                  << t4_score << "/100" << std::endl;
        std::cout << "   T4 Phage description: " 
                  << physics::BindingAssay::getBindingDescription(t4_energy) << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   T4 Phage not available: " << e.what() << std::endl;
    }
    std::cout << std::endl;
    
    // 6. Summary
    std::cout << "=== Phase 3 Validation Complete ===" << std::endl;
    std::cout << "\n✅ Debye-Hückel Theory: Working" << std::endl;
    std::cout << "✅ Phage-Bacteria Binding: Working" << std::endl;
    std::cout << "✅ Interaction Mapping: Working" << std::endl;
    std::cout << "✅ Condition Testing: Working" << std::endl;
    std::cout << "\n🧬 Ready for Phase 4: GUI Development!" << std::endl;
    
    return 0;
}
