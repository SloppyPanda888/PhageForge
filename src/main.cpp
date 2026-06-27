#include "biology/Genome.hpp"
#include "biology/AminoAcid.hpp"
#include "core/Constants.hpp"
#include <iostream>
#include <fmt/format.h>
#include <random>
#include <chrono>

using namespace phageforge;

int main() {
    std::cout << "=== PhageForge Phase 1 Validation ===\n" << std::endl;
    
    // 1. Test Amino Acid Properties Loading
    std::cout << "Loading amino acid properties from config/amino_acids.toml..." << std::endl;
    try {
        biology::AminoAcidPropertiesManager::instance().loadFromTOML("config/amino_acids.toml");
        std::cout << "  ✓ Successfully loaded all 20 amino acids\n" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "  ✗ Failed to load TOML: " << e.what() << std::endl;
        return 1;
    }
    
    // 2. Test Codon Translation
    std::cout << "Testing codon translation (genetic code):" << std::endl;
    auto methionine_codon = biology::Codon::fromString("ATG");
    if (methionine_codon) {
        auto aa = methionine_codon->translate();
        std::cout << fmt::format("  ATG -> {}\n", core::aminoAcidToString(aa)) << std::endl;
    }
    
    // 3. Test Genome Creation and Translation
    std::cout << "Creating a random genome..." << std::endl;
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    auto genome = biology::GenomeFactory::createRandom(rng, 85);
    
    auto amino_acids = genome.translateTailFiber();
    std::cout << fmt::format("  Genome length: {} codons", genome.size()) << std::endl;
    std::cout << fmt::format("  Translated to {} amino acids", amino_acids.size()) << std::endl;
    
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
    std::cout << fmt::format("  T4 tail length: {} amino acids\n", t4_aa.size()) << std::endl;
    
    // 5. Test Mutation
    std::cout << "Testing mutation system..." << std::endl;
    auto mutated = t4_genome;
    mutated.mutateRandom(5, rng);
    auto mutated_aa = mutated.translateTailFiber();
    
    // Count differences
    size_t differences = 0;
    for (size_t i = 0; i < std::min(t4_aa.size(), mutated_aa.size()); ++i) {
        if (t4_aa[i] != mutated_aa[i]) differences++;
    }
    std::cout << fmt::format("  Applied 5 random mutations. Changed {} amino acids.\n", differences) << std::endl;
    
    // 6. Test Serialization
    std::cout << "Testing JSON serialization..." << std::endl;
    auto json = t4_genome.toJSON();
    std::cout << "  JSON output: " << json.dump(2) << std::endl;
    
    auto loaded_genome = biology::Genome::fromJSON(json);
    if (loaded_genome == t4_genome) {
        std::cout << "  ✓ Serialization round-trip successful\n" << std::endl;
    } else {
        std::cout << "  ✗ Serialization mismatch!" << std::endl;
    }
    
    // 7. Summary of Physical Constants
    std::cout << "Physical constants loaded:" << std::endl;
    std::cout << fmt::format("  Temperature: {} K", core::constants::TEMPERATURE) << std::endl;
    std::cout << fmt::format("  Ionic strength: {} M", core::constants::IONIC_STRENGTH) << std::endl;
    std::cout << fmt::format("  Inverse Debye length: {} nm^-1", core::constants::DEBYE_LENGTH_INVERSE) << std::endl;
    std::cout << fmt::format("  Infection threshold: {} kJ/mol", core::constants::INFECTION_THRESHOLD) << std::endl;
    
    std::cout << "\n=== Phase 1 Validation Complete ===" << std::endl;
    return 0;
}
