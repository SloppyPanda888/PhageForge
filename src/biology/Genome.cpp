#include "Genome.hpp"
#include "../core/Exceptions.hpp"
#include <nlohmann/json.hpp>
#include <fmt/format.h>
#include <algorithm>

namespace phageforge::biology {

// --- Genome Implementation ---

Genome::Genome(const std::vector<Codon>& tail_fiber_codons) 
    : m_tail_fiber(tail_fiber_codons) {
    if (tail_fiber_codons.size() > MAX_CODONS) {
        throw core::InvalidArgumentException(
            fmt::format("Too many codons: {} (max {})", 
                       tail_fiber_codons.size(), MAX_CODONS)
        );
    }
}

std::vector<core::AminoAcidCode> Genome::translateTailFiber() const {
    std::vector<core::AminoAcidCode> result;
    result.reserve(m_tail_fiber.size());
    
    for (const auto& codon : m_tail_fiber) {
        auto aa = codon.translate();
        // Stop at first STOP codon (truncated tail fiber)
        if (aa == core::AminoAcidCode::STOP) {
            break;
        }
        result.push_back(aa);
    }
    
    return result;
}

std::string Genome::getDNASequence() const {
    std::string result;
    result.reserve(m_tail_fiber.size() * 3);
    for (const auto& codon : m_tail_fiber) {
        result += codon.toString();
    }
    return result;
}

void Genome::mutatePoint(u32 position, core::DNABase new_base) {
    if (!isValidPosition(position)) {
        throw core::OutOfBoundsException(
            fmt::format("Mutation position {} out of range (size {})", 
                       position, m_tail_fiber.size())
        );
    }
    
    u32 codon_idx = position / 3;
    u32 base_in_codon = position % 3;
    m_tail_fiber[codon_idx].bases[base_in_codon] = new_base;
}

void Genome::mutateCodon(u32 position, const Codon& new_codon) {
    if (!isValidPosition(position)) {
        throw core::OutOfBoundsException(
            fmt::format("Codon mutation position {} out of range (size {})", 
                       position, m_tail_fiber.size())
        );
    }
    m_tail_fiber[position] = new_codon;
}

void Genome::mutateRandom(u32 num_mutations, std::mt19937& rng) {
    std::uniform_int_distribution<u32> codon_dist(0, m_tail_fiber.size() - 1);
    std::uniform_int_distribution<u32> base_dist(0, 2);
    std::uniform_int_distribution<u32> new_base_dist(0, 3);
    
    for (u32 i = 0; i < num_mutations; ++i) {
        u32 codon_idx = codon_dist(rng);
        u32 base_idx = base_dist(rng);
        auto new_base = static_cast<core::DNABase>(new_base_dist(rng));
        
        // Don't mutate to the same base
        if (new_base != m_tail_fiber[codon_idx].bases[base_idx]) {
            m_tail_fiber[codon_idx].bases[base_idx] = new_base;
        }
    }
}

Genome Genome::crossover(const Genome& other, u32 split_point) const {
    if (m_tail_fiber.size() != other.m_tail_fiber.size()) {
        throw core::InvalidArgumentException("Genomes must be same length for crossover");
    }
    
    if (split_point > m_tail_fiber.size()) {
        throw core::OutOfBoundsException("Split point exceeds genome length");
    }
    
    std::vector<Codon> child_codons;
    child_codons.reserve(m_tail_fiber.size());
    
    // First half from this, second half from other
    for (size_t i = 0; i < split_point; ++i) {
        child_codons.push_back(m_tail_fiber[i]);
    }
    for (size_t i = split_point; i < m_tail_fiber.size(); ++i) {
        child_codons.push_back(other.m_tail_fiber[i]);
    }
    
    return Genome(child_codons);
}

bool Genome::isValid() const noexcept {
    if (m_tail_fiber.empty()) return false;
    if (m_tail_fiber.size() > MAX_CODONS) return false;
    
    for (const auto& codon : m_tail_fiber) {
        if (!codon.isValid()) return false;
    }
    return true;
}

nlohmann::json Genome::toJSON() const {
    nlohmann::json j;
    j["tail_fiber_dna"] = getDNASequence();
    j["length"] = m_tail_fiber.size();
    return j;
}

Genome Genome::fromJSON(const nlohmann::json& j) {
    std::string dna = j.at("tail_fiber_dna").get<std::string>();
    
    if (dna.length() % 3 != 0) {
        throw core::SerializationException("DNA sequence length must be multiple of 3");
    }
    
    std::vector<Codon> codons;
    codons.reserve(dna.length() / 3);
    
    for (size_t i = 0; i < dna.length(); i += 3) {
        std::string codon_str = dna.substr(i, 3);
        auto opt_codon = Codon::fromString(codon_str);
        if (!opt_codon) {
            throw core::SerializationException(
                fmt::format("Invalid codon in DNA sequence: {}", codon_str)
            );
        }
        codons.push_back(*opt_codon);
    }
    
    return Genome(codons);
}

// --- GenomeFactory Implementation ---

Genome GenomeFactory::createRandom(std::mt19937& rng, size_t length) {
    if (length > Genome::MAX_CODONS) {
        length = Genome::MAX_CODONS;
    }
    
    std::vector<Codon> codons;
    codons.reserve(length);
    
    std::uniform_int_distribution<u32> base_dist(0, 3);
    
    for (size_t i = 0; i < length; ++i) {
        auto b1 = static_cast<core::DNABase>(base_dist(rng));
        auto b2 = static_cast<core::DNABase>(base_dist(rng));
        auto b3 = static_cast<core::DNABase>(base_dist(rng));
        codons.emplace_back(b1, b2, b3);
    }
    
    return Genome(codons);
}

Genome GenomeFactory::createT4WildType() {
    // This would be the actual T4 phage tail fiber sequence
    // For now, generate a simple sequence with a known binding pattern
    // A real implementation would have 85 codons from NCBI GenBank
    
    std::vector<Codon> codons;
    // Simplified: alternating positive/negative charges for binding
    // Lysine (K = +1), Glutamic Acid (E = -1), alternating
    const std::vector<std::string> t4_sequence = {
        "AAA", "GAA", "AAA", "GAA", "AAA", "GAA", "AAA", "GAA", "AAA", "GAA",
        "AAA", "GAA", "AAA", "GAA", "AAA", "GAA", "AAA", "GAA", "AAA", "GAA"
    };
    
    for (const auto& codon_str : t4_sequence) {
        auto opt_codon = Codon::fromString(codon_str);
        if (opt_codon) {
            codons.push_back(*opt_codon);
        }
    }
    
    return Genome(codons);
}

Genome GenomeFactory::fromAminoAcidSequence(const std::vector<core::AminoAcidCode>& sequence) {
    std::vector<Codon> codons;
    codons.reserve(sequence.size());
    
    // Use the first available codon for each amino acid
    const auto& genetic_code = GeneticCode::instance();
    for (auto aa : sequence) {
        auto codon_set = genetic_code.getCodonsForAminoAcid(aa);
        if (codon_set[0].isValid()) {
            codons.push_back(codon_set[0]);
        } else {
            throw core::InvalidArgumentException(
                fmt::format("No codon found for amino acid {}", 
                           core::aminoAcidToString(aa))
            );
        }
    }
    
    return Genome(codons);
}

} // namespace phageforge::biology
