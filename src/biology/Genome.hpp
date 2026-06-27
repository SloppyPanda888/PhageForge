#pragma once

#include "../core/Types.hpp"
#include "Codon.hpp"
#include "AminoAcid.hpp"
#include <vector>
#include <random>

namespace phageforge::biology {

// A complete phage genome (simplified - only tail fiber genes for Phase 1)
class Genome {
public:
    // Constructors
    Genome() = default;
    explicit Genome(const std::vector<Codon>& tail_fiber_codons);
    
    // Accessors
    [[nodiscard]] const std::vector<Codon>& getTailFiberCodons() const noexcept { return m_tail_fiber; }
    [[nodiscard]] std::vector<core::AminoAcidCode> translateTailFiber() const;
    [[nodiscard]] std::string getDNASequence() const;
    
    // Mutations (for player editing and random generation)
    void mutatePoint(u32 position, core::DNABase new_base);
    void mutateCodon(u32 position, const Codon& new_codon);
    void mutateRandom(u32 num_mutations, std::mt19937& rng);
    
    // Crossover (for future evolution features)
    [[nodiscard]] Genome crossover(const Genome& other, u32 split_point) const;
    
    // Utility
    [[nodiscard]] bool isValid() const noexcept;
    [[nodiscard]] size_t size() const noexcept { return m_tail_fiber.size(); }
    
    // Load/Save to JSON (uses serialization module)
    [[nodiscard]] nlohmann::json toJSON() const;
    static Genome fromJSON(const nlohmann::json& j);
    
    // Comparison
    bool operator==(const Genome& other) const noexcept = default;
    
private:
    std::vector<Codon> m_tail_fiber;  // 512 codons max (170 amino acids)
    static constexpr size_t MAX_CODONS = 512;
    
    // Helper to clamp position
    [[nodiscard]] bool isValidPosition(size_t pos) const noexcept { return pos < m_tail_fiber.size(); }
};

// A factory for generating default genomes
class GenomeFactory {
public:
    // Generate a random genome with realistic codon distribution
    static Genome createRandom(std::mt19937& rng, size_t length = 85);
    
    // Generate the classic T4 phage tail fiber sequence (wild-type)
    static Genome createT4WildType();
    
    // Generate from a protein sequence (3-letter codes)
    static Genome fromAminoAcidSequence(const std::vector<core::AminoAcidCode>& sequence);
};

} // namespace phageforge::biology
