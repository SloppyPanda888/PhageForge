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
    
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Randomize")) randomizeGenome();
            if (ImGui::MenuItem("Clear")) clearGenome();
            ImGui::Separator();
            if (ImGui::MenuItem("Export JSON")) {
                if (m_on_save) m_on_save(m_genome);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Add Random Mutation")) addRandomMutation();
            if (ImGui::MenuItem("Show Codon Table")) m_show_codon_table = !m_show_codon_table;
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
        ImGui::Text("%s...", dna.substr(0, 60).c_str());
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
    
    if (m_show_codon_table) {
        drawCodonTable();
    }
}

void PhageEditor::drawCodonEditor() {
    ImGui::Text("Codon Editor (click to select):");
    
    ImGui::BeginChild("CodonList", ImVec2(0, 180), true);
    
    auto aa_sequence = m_genome.translateTailFiber();
    
    // Display codons with proper amino acid mapping
    int items_per_row = 2;
    int count = 0;
    
    for (size_t i = 0; i < m_genome.size() && i < 50; ++i) {
        ImGui::PushID(static_cast<int>(i));
        
        if (count % items_per_row != 0) {
            ImGui::SameLine();
        }
        
        auto codon = m_genome.getTailFiberCodons()[i];
        std::string codon_str = codon.toString();
        
        // Get amino acid properly
        std::string aa_str;
        ImVec4 color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        
        // Check if this codon translates to an amino acid
        auto aa = codon.translate();
        
        if (aa == core::AminoAcidCode::STOP) {
            aa_str = "STOP";
            color = ImVec4(0.8f, 0.2f, 0.2f, 1.0f); // Red for STOP
        } else {
            // Get the one-letter code
            try {
                auto props = biology::AminoAcidPropertiesManager::instance().getProperties(aa);
                aa_str = props.one_letter;
                color = hexToImVec4(getAminoColor(aa));
            } catch (...) {
                aa_str = "?";
            }
        }
        
        bool selected = (m_selected_codon == static_cast<int>(i));
        
        // Create button label: "ATG → M" or "TAA → STOP"
        std::string button_label = codon_str + " → " + aa_str;
        
        ImGui::PushStyleColor(ImGuiCol_Button, selected ? ImVec4(0.3f, 0.5f, 1.0f, 1.0f) : ImVec4(0.20f, 0.20f, 0.25f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.6f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        
        if (ImGui::Button(button_label.c_str(), ImVec2(130, 30))) {
            m_selected_codon = selected ? -1 : static_cast<int>(i);
        }
        
        ImGui::PopStyleColor(3);
        
        // Show position number
        ImGui::SameLine(0, 4);
        ImGui::TextDisabled("#%zu", i);
        
        // Tooltip on hover
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Position %zu\nCodon: %s\nAmino Acid: %s", 
                i, codon_str.c_str(), aa_str.c_str());
        }
        
        ImGui::PopID();
        count++;
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
    
    std::string aa_name = core::aminoAcidToString(aa);
    ImGui::Text("Selected: %s → %s", codon.toString().c_str(), aa_name.c_str());
    
    if (aa != core::AminoAcidCode::STOP) {
        try {
            auto props = biology::AminoAcidPropertiesManager::instance().getProperties(aa);
            ImGui::Text("Charge: %.2f e | Hydrophobicity: %.2f", 
                props.net_charge_at_ph7, props.hydrophobicity);
        } catch (const std::exception&) {
            ImGui::Text("Properties not available");
        }
    }
}

void PhageEditor::drawMutationControls() {
    ImGui::Text("Mutations:");
    
    float avail_width = ImGui::GetContentRegionAvail().x;
    float button_width = (avail_width - 30.0f) / 4.0f;
    button_width = std::max(70.0f, std::min(100.0f, button_width));
    
    float button_height = 30.0f;
    
    if (ImGui::Button("Randomize", ImVec2(button_width, button_height))) {
        randomizeGenome();
    }
    ImGui::SameLine(0, 8);
    if (ImGui::Button("+1", ImVec2(button_width, button_height))) {
        addRandomMutation();
    }
    ImGui::SameLine(0, 8);
    if (ImGui::Button("+5", ImVec2(button_width, button_height))) {
        for (int i = 0; i < 5; ++i) addRandomMutation();
    }
    ImGui::SameLine(0, 8);
    if (ImGui::Button("Clear", ImVec2(button_width, button_height))) {
        clearGenome();
    }
}

void PhageEditor::drawGenomeStats() {
    auto aa_sequence = m_genome.translateTailFiber();
    double total_charge = 0.0;
    int valid_aa = 0;
    int hydrophobic = 0;
    int hydrophilic = 0;
    int charged = 0;
    int stop_codons = 0;
    
    for (auto aa : aa_sequence) {
        if (aa == core::AminoAcidCode::STOP) {
            stop_codons++;
            continue;
        }
        try {
            auto props = biology::AminoAcidPropertiesManager::instance().getProperties(aa);
            total_charge += props.net_charge_at_ph7;
            if (props.hydrophobicity > 0) hydrophobic++;
            else hydrophilic++;
            if (std::abs(props.net_charge_at_ph7) > 0.5) charged++;
            valid_aa++;
        } catch (...) {}
    }
    
    // Also count STOP codons in the genome
    for (size_t i = 0; i < m_genome.size(); ++i) {
        auto codon = m_genome.getTailFiberCodons()[i];
        if (codon.translate() == core::AminoAcidCode::STOP) {
            // Already counted above, but double-check
        }
    }
    
    ImGui::Text("Valid AA: %d | STOP: %d | Total: %zu", 
        valid_aa, stop_codons, m_genome.size());
    ImGui::Text("Total Charge: %.2f e | Hydrophobic: %d | Charged: %d", 
        total_charge, hydrophobic, charged);
}

void PhageEditor::drawCodonTable() {
    ImGui::Begin("Codon Table", &m_show_codon_table, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Standard Genetic Code");
    ImGui::Separator();
    
    const char* codons[4][4][4] = {
        {{"TTT F", "TCT S", "TAT Y", "TGT C"},
         {"TTC F", "TCC S", "TAC Y", "TGC C"},
         {"TTA L", "TCA S", "TAA STOP", "TGA STOP"},
         {"TTG L", "TCG S", "TAG STOP", "TGG W"}},
        {{"CTT L", "CCT P", "CAT H", "CGT R"},
         {"CTC L", "CCC P", "CAC H", "CGC R"},
         {"CTA L", "CCA P", "CAA Q", "CGA R"},
         {"CTG L", "CCG P", "CAG Q", "CGG R"}},
        {{"ATT I", "ACT T", "AAT N", "AGT S"},
         {"ATC I", "ACC T", "AAC N", "AGC S"},
         {"ATA I", "ACA T", "AAA K", "AGA R"},
         {"ATG M", "ACG T", "AAG K", "AGG R"}},
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
        case core::AminoAcidCode::ALA:
        case core::AminoAcidCode::VAL:
        case core::AminoAcidCode::LEU:
        case core::AminoAcidCode::ILE:
        case core::AminoAcidCode::MET:
        case core::AminoAcidCode::PHE:
        case core::AminoAcidCode::TRP:
        case core::AminoAcidCode::PRO:
            return "#FFD700";
        case core::AminoAcidCode::LYS:
        case core::AminoAcidCode::ARG:
            return "#4169E1";
        case core::AminoAcidCode::ASP:
        case core::AminoAcidCode::GLU:
            return "#DC143C";
        case core::AminoAcidCode::SER:
        case core::AminoAcidCode::THR:
        case core::AminoAcidCode::TYR:
        case core::AminoAcidCode::ASN:
        case core::AminoAcidCode::GLN:
            return "#32CD32";
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
