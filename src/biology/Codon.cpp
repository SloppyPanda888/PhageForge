#include "Codon.hpp"
#include "../core/Exceptions.hpp"
#include "../core/Utilities.hpp"
#include <unordered_map>
#include <optional>
#include <string>

namespace phageforge::biology {

// --- Codon Implementation ---

std::optional<Codon> Codon::fromString(const std::string& str) noexcept {
    if (str.length() != 3) {
        return std::nullopt;
    }
    
    core::DNABase b1 = core::charToBase(str[0]);
    core::DNABase b2 = core::charToBase(str[1]);
    core::DNABase b3 = core::charToBase(str[2]);
    
    if (b1 == core::DNABase::INVALID || b2 == core::DNABase::INVALID || b3 == core::DNABase::INVALID) {
        return std::nullopt;
    }
    
    return Codon{b1, b2, b3};
}

std::string Codon::toString() const noexcept {
    std::string result;
    result.reserve(3);
    result.push_back(core::baseToChar(bases[0]));
    result.push_back(core::baseToChar(bases[1]));
    result.push_back(core::baseToChar(bases[2]));
    return result;
}

core::AminoAcidCode Codon::translate() const noexcept {
    return GeneticCode::instance().lookup(*this);
}

bool Codon::isValid() const noexcept {
    return bases[0] != core::DNABase::INVALID &&
           bases[1] != core::DNABase::INVALID &&
           bases[2] != core::DNABase::INVALID;
}

// --- GeneticCode Implementation ---

constexpr u32 GeneticCode::codonToIndex(core::DNABase b1, core::DNABase b2, core::DNABase b3) noexcept {
    // Base: A=0, C=1, G=2, T=3
    // Index = ((b1 * 4) + b2) * 4 + b3
    return (static_cast<u32>(b1) * 16) + (static_cast<u32>(b2) * 4) + static_cast<u32>(b3);
}

GeneticCode::GeneticCode() {
    // Initialize all to STOP first
    m_translation_table.fill(core::AminoAcidCode::STOP);
    
    // Helper lambda for setting a codon
    auto set = [this](const std::string& codon_str, core::AminoAcidCode aa) {
        auto opt_codon = Codon::fromString(codon_str);
        if (!opt_codon) {
            // Skip invalid codons instead of throwing (no fmt dependency)
            return;
        }
        auto codon = *opt_codon;
        u32 idx = codonToIndex(codon.bases[0], codon.bases[1], codon.bases[2]);
        m_translation_table[idx] = aa;
    };
    
    // --- Standard genetic code ---
    // Alanine
    set("GCT", core::AminoAcidCode::ALA);
    set("GCC", core::AminoAcidCode::ALA);
    set("GCA", core::AminoAcidCode::ALA);
    set("GCG", core::AminoAcidCode::ALA);
    
    // Arginine
    set("CGT", core::AminoAcidCode::ARG);
    set("CGC", core::AminoAcidCode::ARG);
    set("CGA", core::AminoAcidCode::ARG);
    set("CGG", core::AminoAcidCode::ARG);
    set("AGA", core::AminoAcidCode::ARG);
    set("AGG", core::AminoAcidCode::ARG);
    
    // Asparagine
    set("AAT", core::AminoAcidCode::ASN);
    set("AAC", core::AminoAcidCode::ASN);
    
    // Aspartic acid
    set("GAT", core::AminoAcidCode::ASP);
    set("GAC", core::AminoAcidCode::ASP);
    
    // Cysteine
    set("TGT", core::AminoAcidCode::CYS);
    set("TGC", core::AminoAcidCode::CYS);
    
    // Glutamine
    set("CAA", core::AminoAcidCode::GLN);
    set("CAG", core::AminoAcidCode::GLN);
    
    // Glutamic acid
    set("GAA", core::AminoAcidCode::GLU);
    set("GAG", core::AminoAcidCode::GLU);
    
    // Glycine
    set("GGT", core::AminoAcidCode::GLY);
    set("GGC", core::AminoAcidCode::GLY);
    set("GGA", core::AminoAcidCode::GLY);
    set("GGG", core::AminoAcidCode::GLY);
    
    // Histidine
    set("CAT", core::AminoAcidCode::HIS);
    set("CAC", core::AminoAcidCode::HIS);
    
    // Isoleucine
    set("ATT", core::AminoAcidCode::ILE);
    set("ATC", core::AminoAcidCode::ILE);
    set("ATA", core::AminoAcidCode::ILE);
    
    // Leucine
    set("TTA", core::AminoAcidCode::LEU);
    set("TTG", core::AminoAcidCode::LEU);
    set("CTT", core::AminoAcidCode::LEU);
    set("CTC", core::AminoAcidCode::LEU);
    set("CTA", core::AminoAcidCode::LEU);
    set("CTG", core::AminoAcidCode::LEU);
    
    // Lysine
    set("AAA", core::AminoAcidCode::LYS);
    set("AAG", core::AminoAcidCode::LYS);
    
    // Methionine (START)
    set("ATG", core::AminoAcidCode::MET);
    
    // Phenylalanine
    set("TTT", core::AminoAcidCode::PHE);
    set("TTC", core::AminoAcidCode::PHE);
    
    // Proline
    set("CCT", core::AminoAcidCode::PRO);
    set("CCC", core::AminoAcidCode::PRO);
    set("CCA", core::AminoAcidCode::PRO);
    set("CCG", core::AminoAcidCode::PRO);
    
    // Serine
    set("TCT", core::AminoAcidCode::SER);
    set("TCC", core::AminoAcidCode::SER);
    set("TCA", core::AminoAcidCode::SER);
    set("TCG", core::AminoAcidCode::SER);
    set("AGT", core::AminoAcidCode::SER);
    set("AGC", core::AminoAcidCode::SER);
    
    // Threonine
    set("ACT", core::AminoAcidCode::THR);
    set("ACC", core::AminoAcidCode::THR);
    set("ACA", core::AminoAcidCode::THR);
    set("ACG", core::AminoAcidCode::THR);
    
    // Tryptophan
    set("TGG", core::AminoAcidCode::TRP);
    
    // Tyrosine
    set("TAT", core::AminoAcidCode::TYR);
    set("TAC", core::AminoAcidCode::TYR);
    
    // Valine
    set("GTT", core::AminoAcidCode::VAL);
    set("GTC", core::AminoAcidCode::VAL);
    set("GTA", core::AminoAcidCode::VAL);
    set("GTG", core::AminoAcidCode::VAL);
    
    // STOP codons (already set to STOP by default)
    // TAA, TAG, TGA
}

const GeneticCode& GeneticCode::instance() noexcept {
    static GeneticCode code;
    return code;
}

core::AminoAcidCode GeneticCode::lookup(const Codon& codon) const noexcept {
    if (!codon.isValid()) {
        return core::AminoAcidCode::STOP;  // Invalid codons act as terminators
    }
    u32 idx = codonToIndex(codon.bases[0], codon.bases[1], codon.bases[2]);
    return m_translation_table[idx];
}

std::array<Codon, 6> GeneticCode::getCodonsForAminoAcid(core::AminoAcidCode aa) const noexcept {
    std::array<Codon, 6> result;
    size_t count = 0;
    
    // Brute force search (64 entries, called rarely, fine for initialization)
    for (u32 i = 0; i < 64; ++i) {
        if (m_translation_table[i] == aa) {
            // Convert index back to codon
            u32 b1_idx = (i >> 4) & 0x3;  // Bits 4-5
            u32 b2_idx = (i >> 2) & 0x3;  // Bits 2-3
            u32 b3_idx = i & 0x3;         // Bits 0-1
            
            auto b1 = static_cast<core::DNABase>(b1_idx);
            auto b2 = static_cast<core::DNABase>(b2_idx);
            auto b3 = static_cast<core::DNABase>(b3_idx);
            
            result[count++] = Codon{b1, b2, b3};
            
            if (count >= result.size()) break;
        }
    }
    
    // Fill remaining slots with invalid codons
    while (count < result.size()) {
        result[count++] = Codon{};
    }
    
    return result;
}

} // namespace phageforge::biology
