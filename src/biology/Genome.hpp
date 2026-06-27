#pragma once

#include "../core/Types.hpp"
#include "Codon.hpp"
#include <vector>
#include <random>
#include <string>

// Use core types
using phageforge::core::u32;

namespace phageforge::biology {

class Genome {
public:
    static constexpr size_t MAX_CODONS = 512;
    
    Genome() = default;
    explicit Genome(const std::vector<Codon>& tail_fiber_codons);
    
    [[nodiscard]] const std::vector<Codon>& getTailFiberCodons() const noexcept { return m_tail_fiber; }
    [[nodiscard]] std::vector<core::AminoAcidCode> translateTailFiber() const;
    [[nodiscard]] std::string getDNASequence() const;
    
    void mutatePoint(u32 position, core::DNABase new_base);
    void mutateCodon(u32 position, const Codon& new_codon);
    void mutateRandom(u32 num_mutations, std::mt19937& rng);
    
    [[nodiscard]] Genome crossover(const Genome& other, u32 split_point) const;
    
    [[nodiscard]] bool isValid() const noexcept;
    [[nodiscard]] size_t size() const noexcept { return m_tail_fiber.size(); }
    
    // JSON serialization - simplified for Phase 1 (no external JSON library)
    [[nodiscard]] std::string toJSON() const;
    static Genome fromJSON(const std::string& json_str);
    
    bool operator==(const Genome& other) const noexcept = default;
    
private:
    std::vector<Codon> m_tail_fiber;
    
    [[nodiscard]] bool isValidPosition(size_t pos) const noexcept { return pos < m_tail_fiber.size(); }
};

class GenomeFactory {
public:
    static Genome createRandom(std::mt19937& rng, size_t length = 85);
    static Genome createT4WildType();
    static Genome fromAminoAcidSequence(const std::vector<core::AminoAcidCode>& sequence);
};

} // namespace phageforge::biology
