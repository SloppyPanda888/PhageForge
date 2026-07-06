#pragma once

#include "../biology/Genome.hpp"
#include <vector>
#include <string>
#include <functional>

namespace phageforge::gui {

/**
 * @brief Phage Genome Editor GUI Component
 */
class PhageEditor {
public:
    PhageEditor();
    ~PhageEditor() = default;
    
    // Set the genome being edited
    void setGenome(const biology::Genome& genome);
    biology::Genome getGenome() const { return m_genome; }
    
    // Render the editor GUI
    void render();
    
    // Callbacks
    void setOnMutation(std::function<void()> callback) { m_on_mutation = callback; }
    void setOnSave(std::function<void(const biology::Genome&)> callback) { m_on_save = callback; }
    
    // Mutation controls
    void randomizeGenome();
    void addRandomMutation();
    void clearGenome();
    
    // Get selected codon info
    int getSelectedCodon() const { return m_selected_codon; }
    void setSelectedCodon(int index) { m_selected_codon = index; }
    
private:
    biology::Genome m_genome;
    int m_selected_codon = -1;
    bool m_show_codon_table = false;
    std::string m_genome_name = "Unnamed Phage";
    
    // Callbacks
    std::function<void()> m_on_mutation;
    std::function<void(const biology::Genome&)> m_on_save;
    
    // Helper to draw amino acid colors
    void drawCodonEditor();
    void drawAminoAcidInfo();
    void drawMutationControls();
    void drawGenomeStats();
    
    // Color mapping for amino acids
    static const char* getAminoColor(core::AminoAcidCode aa);
};

} // namespace phageforge::gui
