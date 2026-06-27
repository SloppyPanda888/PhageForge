#include "biology/Genome.hpp"
#include "biology/AminoAcid.hpp"
#include "core/Constants.hpp"
#include "core/Exceptions.hpp"
#include "core/Utilities.hpp"  // <-- ADD THIS INCLUDE!
#include <iostream>
#include <random>
#include <chrono>
#include <string>

using namespace phageforge;

int main() {
    std::cout << "=== PhageForge Phase 1 Validation ===\n" << std::endl;
    
    // 1. Test Amino Acid Properties Loading
    std::cout << "Loading amino acid properties..." << std::endl;
    try {
        biology::AminoAcidPropertiesManager::instance().loadFromTOML("config/amino_acids.toml");
        std::cout << "  ✓ Successfully loaded all 20 amino acids\n" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "  ✗ Failed to load: " << e.what() << std::endl;
        return 1;
    }
    
    // 2. Test Codon Translation
    std::cout << "Testing codon translation (genetic code):" << std::endl;
    auto methionine_codon = biology::Codon::fromString("ATG");
    if (methionine_codon) {
        auto aa = methionine_codon->translate();
        std::cout << "  ATG -> " << core::aminoAcidToString(aa) << std::endl << std::endl;
    }
    
    // 3. Test Genome Creation and Translation
    std::cout << "Creating a random genome..." << std::endl;
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    auto genome = biology::GenomeFactory::createRandom(rng, 85);
    
    auto amino_acids = genome.translateTailFiber();
    std::cout << "  Genome length: " << genome.size() << " codons" << std::endl;
    std::cout << "  Translated to " << amino_acids.size() << " amino acids" << std::endl;
    
    // Show first 10 amino acids
    std::cout << "  First 10 amino acids: ";
    for (size_t i = 0; i < std::min(size_t(10), amino_acids.size()); ++i) {
        auto props = biology::AminoAcidPropertiesManager::instance().getProperties(amino_acids[i]);
        std::cout << props.one_letter << " ";
    }
    std::cout << "\n" << std::endl;
    
    // 4. Test T4 Wild Type
    std::cout << "Creating T4 wild-type phage tail fiber..." << std::endl;
    auto t4_genome = biology::GenomeFactory::createT4WildType();
    auto t4_aa = t4_genome.translateTailFiber();
    std::cout << "  T4 tail length: " << t4_aa.size() << " amino acids\n" << std::endl;
    
    // 5. Test Mutation
    std::cout << "Testing mutation system..." << std::endl;
    auto mutated = t4_genome;
    mutated.mutateRandom(5, rng);
    auto mutated_aa = mutated.translateTailFiber();
    
    size_t differences = 0;
    for (size_t i = 0; i < std::min(t4_aa.size(), mutated_aa.size()); ++i) {
        if (t4_aa[i] != mutated_aa[i]) differences++;
    }
    std::cout << "  Applied 5 random mutations. Changed " << differences << " amino acids.\n" << std::endl;
    
    // 6. Test Serialization
    std::cout << "Testing JSON serialization..." << std::endl;
    std::string json = t4_genome.toJSON();
    std::cout << "  JSON output: " << json << std::endl;
    
    auto loaded_genome = biology::Genome::fromJSON(json);
    if (loaded_genome == t4_genome) {
        std::cout << "  ✓ Serialization round-trip successful\n" << std::endl;
    } else {
        std::cout << "  ✗ Serialization mismatch!" << std::endl;
    }
    
    // 7. Summary of Physical Constants
    std::cout << "Physical constants loaded:" << std::endl;
    std::cout << "  Temperature: " << core::constants::TEMPERATURE << " K" << std::endl;
    std::cout << "  Ionic strength: " << core::constants::IONIC_STRENGTH << " M" << std::endl;
    std::cout << "  Inverse Debye length: " << core::constants::DEBYE_LENGTH_INVERSE << " nm^-1" << std::endl;
    std::cout << "  Infection threshold: " << core::constants::INFECTION_THRESHOLD << " kJ/mol" << std::endl;
    
    std::cout << "\n=== Phase 1 Validation Complete ===" << std::endl;
    return 0;
}
