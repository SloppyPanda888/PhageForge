#include "PhageEditor.hpp"
#include "../physics/Electrostatics.hpp"
#include "../biology/AminoAcid.hpp"
#include "../core/Utilities.hpp"

#include <imgui.h>
#include <random>
#include <chrono>
#include <map>
#include <cstdio>

namespace phageforge::gui {

// Helper function to convert hex color to ImVec4 (only in cpp)
static ImVec4 hexToImVec4(const char* hex) {
    if (hex[0] == '#') {
        unsigned int r, g, b;
        sscanf(hex + 1, "%02x%02x%02x", &r, &g, &b);
        return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
    }
    return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}

PhageEditor::PhageEditor() {
    randomizeGenome();
}

void PhageEditor::setGenome(const biology::Genome& genome) {
    m_genome = genome;
}

void PhageEditor::render() {
    ImGui::Begin("Phage Genome Editor", nullptr, ImGuiWindowFlags_MenuBar);
    
    // Menu bar
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Randomize")) {
                randomizeGenome();
            }
            if (ImGui::MenuItem("Clear")) {
                clearGenome();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Export JSON")) {
                if (m_on_save) m_on_save(m_genome);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Add Random Mutation")) {
                addRandomMutation();
            }
            if (ImGui::MenuItem("Show Codon Table")) {
                m_show_codon_table = !m_show_codon_table;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    
    // Genome info
    ImGui::Text("Genome: %s", m_genome_name.c_str());
    ImGui::Text("Length: %zu codons (%zu amino acids)", 
                m_genome.size(), 
                m_genome.translateTailFiber().size());
    
    // DNA sequence
    ImGui::Separator();
    ImGui::Text("DNA Sequence:");
    std::string dna = m_genome.getDNASequence();
    if (dna.length() > 60) {
        for (size_t i = 0; i < dna.length(); i += 60) {
            size_t len = std::min(size_t(60), dna.length() - i);
            ImGui::Text("%s", dna.substr(i, len).c_str());
        }
    } else {
        ImGui::Text("%s", dna.c_str());
    }
    
    ImGui::Separator();
    drawCodonEditor();
    ImGui::Separator();
    drawAminoAcidInfo();
    ImGui::Separator();
    drawMutationControls();
    drawGenomeStats();
    
    ImGui::End();
    
    // Codon table popup
    if (m_show_codon_table) {
        drawCodonTable();
    }
}

void PhageEditor::drawCodonEditor() {
    ImGui::Text("Codon Editor:");
    ImGui::BeginChild("CodonList", ImVec2(0, 200), true);
    
    auto aa_sequence = m_genome.translateTailFiber();
    
    for (size_t i = 0; i < m_genome.size() && i < 50; ++i) {
        ImGui::PushID(static_cast<int>(i));
        
        // Codon
        std::string codon_str = m_genome.getTailFiberCodons()[i].toString();
        
        // Amino acid
        std::string aa_str = "STOP";
        ImVec4 color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        if (i < aa_sequence.size() && aa_sequence[i] != core::AminoAcidCode::STOP) {
            auto aa = aa_sequence[i];
            try {
                auto props = biology::AminoAcidPropertiesManager::instance().getProperties(aa);
                aa_str = props.one_letter;
                color = hexToImVec4(getAminoColor(aa));
            } catch (...) {
                aa_str = "?";
            }
        }
        
        // Selectable codon with color
        bool selected = (m_selected_codon == static_cast<int>(i));
        ImGui::PushStyleColor(ImGuiCol_Button, selected ? ImVec4(0.3f, 0.5f, 1.0f, 1.0f) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        if (ImGui::Button(codon_str.c_str(), ImVec2(50, 25))) {
            m_selected_codon = selected ? -1 : static_cast<int>(i);
        }
        ImGui::PopStyleColor();
        
        ImGui::SameLine();
        ImGui::Text("→");
        ImGui::SameLine();
        
        ImGui::TextColored(color, "%s", aa_str.c_str());
        ImGui::SameLine();
        ImGui::TextDisabled("#%zu", i);
        
        ImGui::PopID();
    }
    
    ImGui::EndChild();
}

void PhageEditor::drawAminoAcidInfo() {
    if (m_selected_codon < 0 || m_selected_codon >= static_cast<int>(m_genome.size())) {
        ImGui::Text("Select a codon to view details");
        return;
    }
    
    auto codon = m_genome.getTailFiberCodons()[m_selected_codon];
    auto aa = codon.translate();
    
    ImGui::Text("Selected Codon: %s", codon.toString().c_str());
    ImGui::Text("Amino Acid: %s", core::aminoAcidToString(aa).c_str());
    
    if (aa != core::AminoAcidCode::STOP) {
        try {
            auto props = biology::AminoAcidPropertiesManager::instance().getProperties(aa);
            ImGui::Text("Charge: %.2f e", props.net_charge_at_ph7);
            ImGui::Text("Hydrophobicity: %.2f", props.hydrophobicity);
            ImGui::Text("Molecular Weight: %.2f Da", props.molecular_weight);
            ImGui::Text("Van der Waals Radius: %.2f nm", props.van_der_waals_radius);
        } catch (const std::exception&) {
            ImGui::Text("Properties not available");
        }
    }
}

void PhageEditor::drawMutationControls() {
    ImGui::Text("Mutations:");
    
    if (ImGui::Button("Randomize")) {
        randomizeGenome();
    }
    ImGui::SameLine();
    
    if (ImGui::Button("+1 Mutation")) {
        addRandomMutation();
    }
    ImGui::SameLine();
    
    if (ImGui::Button("+5 Mutations")) {
        for (int i = 0; i < 5; ++i) addRandomMutation();
    }
    ImGui::SameLine();
    
    if (ImGui::Button("Clear")) {
        clearGenome();
    }
}

void PhageEditor::drawGenomeStats() {
    ImGui::Text("Genome Statistics:");
    
    auto aa_sequence = m_genome.translateTailFiber();
    
    // Count amino acids
    std::map<char, int> aa_counts;
    double total_charge = 0.0;
    double total_hydrophobicity = 0.0;
    int valid_aa = 0;
    
    for (auto aa : aa_sequence) {
        if (aa != core::AminoAcidCode::STOP) {
            try {
                auto props = biology::AminoAcidPropertiesManager::instance().getProperties(aa);
                aa_counts[props.one_letter[0]]++;
                total_charge += props.net_charge_at_ph7;
                total_hydrophobicity += props.hydrophobicity;
                valid_aa++;
            } catch (const std::exception&) {}
        }
    }
    
    ImGui::Text("Total Charge: %.2f e", total_charge);
    ImGui::Text("Avg Hydrophobicity: %.2f", valid_aa > 0 ? total_hydrophobicity / valid_aa : 0.0);
    ImGui::Text("Unique Amino Acids: %zu", aa_counts.size());
    
    // Show counts as a bar
    std::string count_str;
    for (const auto& [aa, count] : aa_counts) {
        if (count > 0) {
            count_str += std::string(1, aa) + ":" + std::to_string(count) + " ";
        }
    }
    if (!count_str.empty()) {
        ImGui::Text("Composition: %s", count_str.c_str());
    }
}

void PhageEditor::drawCodonTable() {
    ImGui::Begin("Codon Table", &m_show_codon_table, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Standard Genetic Code");
    ImGui::Separator();
    
    // Simplified codon table
    const char* codons[4][4][4] = {
        // T
        {{"TTT F", "TCT S", "TAT Y", "TGT C"},
         {"TTC F", "TCC S", "TAC Y", "TGC C"},
         {"TTA L", "TCA S", "TAA STOP", "TGA STOP"},
         {"TTG L", "TCG S", "TAG STOP", "TGG W"}},
        // C
        {{"CTT L", "CCT P", "CAT H", "CGT R"},
         {"CTC L", "CCC P", "CAC H", "CGC R"},
         {"CTA L", "CCA P", "CAA Q", "CGA R"},
         {"CTG L", "CCG P", "CAG Q", "CGG R"}},
        // A
        {{"ATT I", "ACT T", "AAT N", "AGT S"},
         {"ATC I", "ACC T", "AAC N", "AGC S"},
         {"ATA I", "ACA T", "AAA K", "AGA R"},
         {"ATG M", "ACG T", "AAG K", "AGG R"}},
        // G
        {{"GTT V", "GCT A", "GAT D", "GGT G"},
         {"GTC V", "GCC A", "GAC D", "GGC G"},
         {"GTA V", "GCA A", "GAA E", "GGA G"},
         {"GTG V", "GCG A", "GAG E", "GGG G"}}
    };
    
    const char* bases[4] = {"T", "C", "A", "G"};
    
    ImGui::Columns(5);
    ImGui::Text(""); ImGui::NextColumn();
    ImGui::Text("T"); ImGui::NextColumn();
    ImGui::Text("C"); ImGui::NextColumn();
    ImGui::Text("A"); ImGui::NextColumn();
    ImGui::Text("G"); ImGui::NextColumn();
    
    for (int i = 0; i < 4; ++i) {
        ImGui::Text("%s", bases[i]); ImGui::NextColumn();
        for (int j = 0; j < 4; ++j) {
            ImGui::Text("%s", codons[i][j][0]); ImGui::NextColumn();
        }
    }
    
    ImGui::Columns(1);
    ImGui::End();
}

const char* PhageEditor::getAminoColor(core::AminoAcidCode aa) {
    switch(aa) {
        // Hydrophobic (yellow/orange)
        case core::AminoAcidCode::ALA:
        case core::AminoAcidCode::VAL:
        case core::AminoAcidCode::LEU:
        case core::AminoAcidCode::ILE:
        case core::AminoAcidCode::MET:
        case core::AminoAcidCode::PHE:
        case core::AminoAcidCode::TRP:
        case core::AminoAcidCode::PRO:
            return "#FFD700";
        // Charged positive (blue)
        case core::AminoAcidCode::LYS:
        case core::AminoAcidCode::ARG:
            return "#4169E1";
        // Charged negative (red)
        case core::AminoAcidCode::ASP:
        case core::AminoAcidCode::GLU:
            return "#DC143C";
        // Polar (green)
        case core::AminoAcidCode::SER:
        case core::AminoAcidCode::THR:
        case core::AminoAcidCode::TYR:
        case core::AminoAcidCode::ASN:
        case core::AminoAcidCode::GLN:
            return "#32CD32";
        // Special
        case core::AminoAcidCode::CYS:
            return "#FF8C00";
        case core::AminoAcidCode::GLY:
            return "#D3D3D3";
        case core::AminoAcidCode::HIS:
            return "#8A2BE2";
        default:
            return "#FFFFFF";
    }
}

void PhageEditor::randomizeGenome() {
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    m_genome = biology::GenomeFactory::createRandom(rng, 20);
    if (m_on_mutation) m_on_mutation();
}

void PhageEditor::addRandomMutation() {
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    m_genome.mutateRandom(1, rng);
    if (m_on_mutation) m_on_mutation();
}

void PhageEditor::clearGenome() {
    m_genome = biology::Genome();
    if (m_on_mutation) m_on_mutation();
}

} // namespace phageforge::gui
