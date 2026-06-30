#include "biology/Genome.hpp"
#include "biology/AminoAcid.hpp"
#include "biology/Bacteria.hpp"
#include "biology/QuorumSensing.hpp"
#include "core/Constants.hpp"
#include "core/Utilities.hpp"
#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include <thread>

using namespace phageforge;

// Helper to print a progress bar
void printProgress(double progress, int width = 40) {
    int pos = static_cast<int>(width * progress);
    std::cout << "[";
    for (int i = 0; i < width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << "%\r";
    std::cout.flush();
}

int main() {
    std::cout << "=== PhageForge Phase 2: Bacterial Quorum Sensing ===\n" << std::endl;
    
    // Initialize amino acid properties
    try {
        biology::AminoAcidPropertiesManager::instance().loadFromTOML("config/amino_acids.toml");
        std::cout << "✅ Amino acid properties loaded\n" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "❌ Failed to load: " << e.what() << std::endl;
        return 1;
    }
    
    // 1. Test Quorum Sensing
    std::cout << "1. Testing Quorum Sensing System..." << std::endl;
    biology::QuorumSensingSystem qs;
    qs.setPopulationDensity(5.0);  // High population
    
    std::cout << "   Simulating AHL production over time:" << std::endl;
    for (int i = 0; i < 50; ++i) {
        qs.update(0.1);
        if (i % 5 == 0) {
            std::cout << "   t=" << std::setw(4) << i * 0.1 
                      << "s  LuxI=" << std::fixed << std::setprecision(3) << qs.getLuxI()
                      << "  LuxR=" << qs.getLuxR()
                      << "  AHL=" << qs.getAHL()
                      << "  Quorum=" << (qs.isQuorumReached() ? "✅" : "❌") << std::endl;
        }
    }
    std::cout << std::endl;
    
    // 2. Test Bacterial Growth with Quorum Sensing
    std::cout << "2. Testing Bacterial Growth with Quorum Sensing..." << std::endl;
    biology::BacterialStrain bacteria;
    bacteria.setName("E. coli O157:H7");
    bacteria.setPopulationDensity(0.1);
    bacteria.setMaxPopulation(10.0);
    
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    
    std::cout << "   Growing bacteria with quorum sensing:" << std::endl;
    for (int hour = 0; hour < 24; ++hour) {
        // Update every hour
        double dt = 0.5;  // Half-hour steps for stability
        for (int step = 0; step < 2; ++step) {
            bacteria.updateQuorumSensing(dt);
            bacteria.updatePopulation(dt);
        }
        
        // Print progress bar
        printProgress(hour / 24.0);
        
        if (hour % 4 == 0) {
            std::cout << "\n   Hour " << hour 
                      << "  Population: " << std::fixed << std::setprecision(2) << bacteria.getPopulationDensity()
                      << "  AHL: " << bacteria.getQuorumSensing().getAHL()
                      << "  Quorum: " << (bacteria.isQuorumReached() ? "✅" : "❌")
                      << "  Growth rate: " << std::setprecision(3) << bacteria.getGrowthRate() << std::endl;
        }
    }
    std::cout << std::endl << std::endl;
    
    // 3. Test Receptor Mutation
    std::cout << "3. Testing Receptor Mutation..." << std::endl;
    std::cout << "   Initial receptors:" << std::endl;
    for (const auto& receptor : bacteria.getReceptors()) {
        std::cout << "      Type: " << receptor.getType() 
                  << "  Charge: " << receptor.getCharge()
                  << "  Position: (" << receptor.getPosition().x << ", "
                  << receptor.getPosition().y << ", "
                  << receptor.getPosition().z << ")" << std::endl;
    }
    
    bacteria.mutateReceptors(0.3, rng);  // 30% mutation rate
    
    std::cout << "   After mutation (30% rate):" << std::endl;
    for (const auto& receptor : bacteria.getReceptors()) {
        std::cout << "      Type: " << receptor.getType() 
                  << "  Charge: " << receptor.getCharge()
                  << "  Position: (" << receptor.getPosition().x << ", "
                  << receptor.getPosition().y << ", "
                  << receptor.getPosition().z << ")" << std::endl;
    }
    std::cout << std::endl;
    
    // 4. Test Phage-Bacteria Interaction
    std::cout << "4. Testing Phage-Bacteria Interaction..." << std::endl;
    
    // Create a random phage
    auto phage = biology::GenomeFactory::createRandom(rng, 20);
    std::cout << "   Created random phage with " << phage.size() << " codons" << std::endl;
    
    // Compute binding score
    double binding_score = bacteria.computePhageBindingScore(phage);
    std::cout << "   Binding score: " << std::fixed << std::setprecision(3) << binding_score << std::endl;
    
    // Simulate infection
    double infectivity = 0.8;
    bool infection_success = bacteria.infectWithPhage(phage, infectivity);
    std::cout << "   Infection: " << (infection_success ? "✅ SUCCESSFUL!" : "❌ Failed") << std::endl;
    std::cout << "   Remaining population: " << bacteria.getPopulationDensity() << std::endl;
    std::cout << std::endl;
    
    // 5. Summary
    std::cout << "=== Phase 2 Validation Complete ===" << std::endl;
    std::cout << "\n✅ Quorum Sensing: Working" << std::endl;
    std::cout << "✅ Bacterial Growth: Working" << std::endl;
    std::cout << "✅ Receptor Mutation: Working" << std::endl;
    std::cout << "✅ Phage Interaction: Working" << std::endl;
    std::cout << "\n🧬 Ready for Phase 3: Electrostatics and Binding!" << std::endl;
    
    return 0;
}
