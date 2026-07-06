#include "PhageEditor.hpp"
#include "../physics/Electrostatics.hpp"
#include <imgui.h>
#include <random>
#include <chrono>

namespace phageforge::gui {

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
    ImGui::PushFont(ImGui::GetFont());
    std::string dna = m_genome.getDNASequence();
    if (dna.length() > 60) {
        // Break into lines
        for (size_t i = 0; i < dna.length(); i += 60) {
            size_t len = std::min(size_t(60), dna.length() - i);
            ImGui::Text("%s", dna.substr(i, len).c_str());
        }
    } else {
        ImGui::Text("%s", dna.c_str());
    }
    ImGui::PopFont();
    
    ImGui::Separator();
    
    // Codon editor
    drawCodonEditor();
    
    ImGui::Separator();
    
    // Amino acid info
    drawAminoAcidInfo();
    
    ImGui::Separator();
    
    // Mutation controls
    drawMutationControls();
    
    // Genome stats
    drawGenomeStats();
    
    ImGui::End();
    
    // Codon table popup
    if (m_show_codon_table) {
        ImGui::Begin("Codon Table", &m_show_codon_table);
        // Display codon table (simplified)
        ImGui::Text("Standard Genetic Code");
        ImGui::Columns(4);
        ImGui::Text("TTT F"); ImGui::NextColumn();
        ImGui::Text("TCT S"); ImGui::NextColumn();
        ImGui::Text("TAT Y"); ImGui::NextColumn();
        ImGui::Text("TGT C"); ImGui::NextColumn();
        // ... (add full table)
        ImGui::Columns(1);
        ImGui::End();
    }
}

void PhageEditor::drawCodonEditor() {
    ImGui::Text("Codon Editor:");
    ImGui::BeginChild("CodonList", ImVec2(0, 300), true);
    
    auto aa_sequence = m_genome.translateTailFiber();
    
    for (size_t i = 0; i < m_genome.size() && i < 50; ++i) {
        ImGui::PushID(i);
        
        // Codon
        std::string codon_str = m_genome.getTailFiberCodons()[i].toString();
        
        // Amino acid
        std::string aa_str = "STOP";
        const char* color = "";
        if (i < aa_sequence.size()) {
            auto props = biology::AminoAcidPropertiesManager::instance()
                .getProperties(aa_sequence[i]);
            aa_str = props.one_letter;
            color = getAminoColor(aa_sequence[i]);
        }
        
        // Selectable codon
        bool selected = (m_selected_codon == static_cast<int>(i));
        ImGui::PushStyleColor(ImGuiCol_Button, selected ? ImVec4(0.3f, 0.5f, 1.0f, 1.0f) : ImVec4(0, 0, 0, 0));
        if (ImGui::Button(codon_str.c_str(), ImVec2(50, 25))) {
            m_selected_codon = selected ? -1 : static_cast<int>(i);
        }
        ImGui::PopStyleColor();
        
        ImGui::SameLine();
        
        // Amino acid with color
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "→");
        ImGui::SameLine();
        
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", aa_str.c_str());
        ImGui::SameLine();
        ImGui::Text("  ");
        
        // Position
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
            auto props = biology::AminoAcidPropertiesManager::instance()
                .getProperties(aa);
            
            ImGui::Text("Charge: %.2f", props.net_charge_at_ph7);
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
    
    if (ImGui::Button("Randomize Genome")) {
        randomizeGenome();
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Add 1 Mutation")) {
        addRandomMutation();
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Add 5 Mutations")) {
        for (int i = 0; i < 5; ++i) addRandomMutation();
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Clear Genome")) {
        clearGenome();
    }
}

void PhageEditor::drawGenomeStats() {
    ImGui::Text("Genome Statistics:");
    
    auto aa_sequence = m_genome.translateTailFiber();
    
    // Count amino acids
    std::map<char, int> aa_counts;
    for (auto aa : aa_sequence) {
        if (aa != core::AminoAcidCode::STOP) {
            try {
                auto props = biology::AminoAcidPropertiesManager::instance()
                    .getProperties(aa);
                aa_counts[props.one_letter[0]]++;
            } catch (const std::exception&) {}
        }
    }
    
    // Calculate charge
    double total_charge = 0.0;
    for (auto aa : aa_sequence) {
        if (aa != core::AminoAcidCode::STOP) {
            try {
                auto props = biology::AminoAcidPropertiesManager::instance()
                    .getProperties(aa);
                total_charge += props.net_charge_at_ph7;
            } catch (const std::exception&) {}
        }
    }
    
    ImGui::Text("Total Charge: %.2f", total_charge);
    ImGui::Text("Unique Amino Acids: %zu", aa_counts.size());
    
    // Show counts
    std::string count_str;
    for (const auto& [aa, count] : aa_counts) {
        if (count > 0) {
            count_str += std::string(1, aa) + ":" + std::to_string(count) + " ";
        }
    }
    ImGui::Text("Composition: %s", count_str.c_str());
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
        // Charged (blue/red)
        case core::AminoAcidCode::LYS:
        case core::AminoAcidCode::ARG:
            return "#4169E1";  // Blue (positive)
        case core::AminoAcidCode::ASP:
        case core::AminoAcidCode::GLU:
            return "#DC143C";  // Red (negative)
        // Polar (green)
        case core::AminoAcidCode::SER:
        case core::AminoAcidCode::THR:
        case core::AminoAcidCode::TYR:
        case core::AminoAcidCode::ASN:
        case core::AminoAcidCode::GLN:
            return "#32CD32";
        // Special
        case core::AminoAcidCode::CYS:
            return "#FF8C00";  // Orange
        case core::AminoAcidCode::GLY:
            return "#D3D3D3";  // Light gray
        case core::AminoAcidCode::HIS:
            return "#8A2BE2";  // Purple
        default:
            return "#FFFFFF";
    }
}

} // namespace phageforge::gui
