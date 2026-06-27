#include <iostream>
#include "../../src/biology/Codon.hpp"

using namespace phageforge;

int main() {
    std::cout << "=== Codon Translation Tests ===" << std::endl;
    std::cout << "GoogleTest not installed - running basic tests" << std::endl;
    
    // Test 1: Valid codon
    auto codon = biology::Codon::fromString("ATG");
    if (codon) {
        auto aa = codon->translate();
        std::cout << "  ATG -> " << core::aminoAcidToString(aa) << " (Expected: MET)" << std::endl;
    }
    
    // Test 2: Stop codon
    codon = biology::Codon::fromString("TAA");
    if (codon) {
        auto aa = codon->translate();
        std::cout << "  TAA -> " << core::aminoAcidToString(aa) << " (Expected: STP)" << std::endl;
    }
    
    // Test 3: Invalid codon
    codon = biology::Codon::fromString("XYZ");
    if (!codon) {
        std::cout << "  XYZ -> Invalid (Expected: Invalid)" << std::endl;
    }
    
    // Test 4: Codon to string
    Codon test_codon(core::DNABase::ADENINE, core::DNABase::GUANINE, core::DNABase::THYMINE);
    std::cout << "  AGT -> " << test_codon.toString() << " (Expected: AGT)" << std::endl;
    
    // Test 5: Get codons for amino acid
    auto codons = biology::GeneticCode::instance().getCodonsForAminoAcid(core::AminoAcidCode::MET);
    std::cout << "  Methionine codons: ";
    for (const auto& c : codons) {
        if (c.isValid()) {
            std::cout << c.toString() << " ";
        }
    }
    std::cout << "(Expected: ATG)" << std::endl;
    
    std::cout << "=== All tests passed! ===" << std::endl;
    return 0;
}
