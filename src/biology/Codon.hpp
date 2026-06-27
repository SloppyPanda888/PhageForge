#pragma once

#include "../core/Types.hpp"
#include <array>

namespace phageforge::biology {

// A codon is exactly 3 DNA bases
struct Codon {
    core::DNABase bases[core::NUM_BASES_PER_CODON];
    
    // Constructors
    constexpr Codon() noexcept : bases{core::DNABase::INVALID, core::DNABase::INVALID, core::DNABase::INVALID} {}
    constexpr Codon(core::DNABase b1, core::DNABase b2, core::DNABase b3) noexcept 
        : bases{b1, b2, b3} {}
    
    // Create from string (e.g., "ATG")
    [[nodiscard]] static std::optional<Codon> fromString(const std::string& str) noexcept;
    
    // Convert to string (e.g., "ATG")
    [[nodiscard]] std::string toString() const noexcept;
    
    // Translate codon to amino acid (STOP codon if termination)
    [[nodiscard]] core::AminoAcidCode translate() const noexcept;
    
    // Check if this is a valid codon (no INVALID bases)
    [[nodiscard]] bool isValid() const noexcept;
    
    // Equality operator for testing
    bool operator==(const Codon& other) const noexcept = default;
};

// The standard genetic code (codon -> amino acid)
// This is a 64-entry lookup table
class GeneticCode {
public:
    // Singleton access
    static const GeneticCode& instance() noexcept;
    
    // Lookup a codon's amino acid
    [[nodiscard]] core::AminoAcidCode lookup(const Codon& codon) const noexcept;
    
    // Get all codons that encode a given amino acid (for mutation targeting)
    [[nodiscard]] std::array<Codon, 6> getCodonsForAminoAcid(core::AminoAcidCode aa) const noexcept;
    
private:
    GeneticCode();  // Private constructor for singleton
    
    // Translation table: 64 entries (4^3 possible codons)
    std::array<core::AminoAcidCode, 64> m_translation_table;
    
    // Helper: convert codon to index (0-63)
    [[nodiscard]] static constexpr u32 codonToIndex(core::DNABase b1, core::DNABase b2, core::DNABase b3) noexcept;
};

} // namespace phageforge::biology
