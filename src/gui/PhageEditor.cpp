#include "PhageEditor.hpp"
#include "../physics/Electrostatics.hpp"
#include "../biology/AminoAcid.hpp"

// Include ImGui only in the .cpp file
#include <imgui.h>
#include <random>
#include <chrono>
#include <map>

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
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Add Random Mutation")) {
                addRandomMutation();
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
    
    ImGui::Separator();
    drawCodonEditor();
    ImGui::Separator();
    drawAminoAcidInfo();
    ImGui::Separator();
    drawMutationControls();
    drawGenomeStats();
    
    ImGui::End();
}

void PhageEditor::drawCodonEditor() {
    ImGui::Text("Codon Editor:");
    ImGui::BeginChild("CodonList", ImVec2(0, 150), true);
    
    auto aa_sequence = m_genome.translateTailFiber();
    
    for (size_t i = 0; i < m_genome.size() && i < 30; ++i) {
        ImGui::PushID(static_cast<int>(i));
        std::string codon_str = m_genome.getTailFiberCodons()[i].toString();
        std::string aa_str = (i < aa_sequence.size()) ? 
            core::aminoAcidToString(aa_sequence[i]) : "STOP";
        
        bool selected = (m_selected_codon == static_cast<int>(i));
        if (ImGui::Selectable(codon_str.c_str(), selected)) {
            m_selected_codon = selected ? -1 : static_cast<int>(i);
        }
        ImGui::SameLine();
        ImGui::Text("→ %s", aa_str.c_str());
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
    ImGui::Text("Selected: %s → %s", codon.toString().c_str(), 
                core::aminoAcidToString(aa).c_str());
}

void PhageEditor::drawMutationControls() {
    ImGui::Text("Mutations:");
    if (ImGui::Button("Randomize")) randomizeGenome();
    ImGui::SameLine();
    if (ImGui::Button("+1")) addRandomMutation();
    ImGui::SameLine();
    if (ImGui::Button("+5")) {
        for (int i = 0; i < 5; ++i) addRandomMutation();
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear")) clearGenome();
}

void PhageEditor::drawGenomeStats() {
    auto aa_sequence = m_genome.translateTailFiber();
    double total_charge = 0.0;
    for (auto aa : aa_sequence) {
        if (aa != core::AminoAcidCode::STOP) {
            try {
                auto props = biology::AminoAcidPropertiesManager::instance().getProperties(aa);
                total_charge += props.net_charge_at_ph7;
            } catch (...) {}
        }
    }
    ImGui::Text("Total Charge: %.2f", total_charge);
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
