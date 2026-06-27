#include <iostream>
#include <random>
#include <chrono>
#include "../../src/biology/Genome.hpp"
#include "../../src/biology/AminoAcid.hpp"
#include "../../src/core/Utilities.hpp"

using namespace phageforge;

int main() {
    std::cout << "=== Genome Tests ===" << std::endl;
    std::cout << "GoogleTest not installed - running basic tests" << std::endl;
    
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    
    // Test 1: Create random genome
    biology::Genome genome = biology::GenomeFactory::createRandom(rng, 10);
    std::cout << "  Created random genome: " << genome.size() << " codons" << std::endl;
    
    // Test 2: Translate genome
    std::vector<core::AminoAcidCode> aa_sequence = genome.translateTailFiber();
    std::cout << "  Translated to " << aa_sequence.size() << " amino acids" << std::endl;
    
    // Test 3: Mutation
    biology::Genome mutated = genome;
    mutated.mutateRandom(3, rng);
    std::vector<core::AminoAcidCode> mutated_aa = mutated.translateTailFiber();
    std::cout << "  After 3 mutations: " << mutated_aa.size() << " amino acids" << std::endl;
    
    // Test 4: DNA sequence
    std::cout << "  DNA sequence: " << genome.getDNASequence() << std::endl;
    
    // Test 5: T4 wild type
    biology::Genome t4 = biology::GenomeFactory::createT4WildType();
    std::cout << "  T4 wild type length: " << t4.size() << " codons" << std::endl;
    
    // Test 6: JSON serialization
    std::string json = genome.toJSON();
    std::cout << "  JSON: " << json << std::endl;
    
    try {
        biology::Genome loaded = biology::Genome::fromJSON(json);
        if (loaded == genome) {
            std::cout << "  ✓ Serialization round-trip successful" << std::endl;
        } else {
            std::cout << "  ✗ Serialization round-trip failed!" << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cout << "  ✗ Serialization error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "=== All tests passed! ===" << std::endl;
    return 0;
}
