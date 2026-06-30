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
    std::cout << "=== PhageForge Phase 3: Electrostatics and Binding ===\n" << std::endl;
    
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
    
    std::cout << "   Debye length: " << std::fixed << std
