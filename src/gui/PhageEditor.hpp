#pragma once

#include "../biology/Genome.hpp"
#include <functional>
#include <string>

// Forward declaration (no include needed in header)
struct ImGuiContext;

namespace phageforge::gui {

class PhageEditor {
public:
    PhageEditor();
    ~PhageEditor() = default;
    
    void setGenome(const biology::Genome& genome);
    biology::Genome getGenome() const { return m_genome; }
    
    void render();
    
    void setOnMutation(std::function<void()> callback) { m_on_mutation = callback; }
    void setOnSave(std::function<void(const biology::Genome&)> callback) { m_on_save = callback; }
    
    void randomizeGenome();
    void addRandomMutation();
    void clearGenome();
    
    int getSelectedCodon() const { return m_selected_codon; }
    
private:
    biology::Genome m_genome;
    int m_selected_codon = -1;
    bool m_show_codon_table = false;
    std::string m_genome_name = "Unnamed Phage";
    
    std::function<void()> m_on_mutation;
    std::function<void(const biology::Genome&)> m_on_save;
    
    void drawCodonEditor();
    void drawAminoAcidInfo();
    void drawMutationControls();
    void drawGenomeStats();
};

} // namespace phageforge::gui
