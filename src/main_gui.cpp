#include "gui/PhageEditor.hpp"
#include "physics/Electrostatics.hpp"
#include "physics/BindingAssay.hpp"
#include "biology/AminoAcid.hpp"
#include "biology/Bacteria.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <chrono>

using namespace phageforge;

// Global state
struct AppState {
    biology::Genome phage_genome;
    biology::BacterialStrain bacteria;
    gui::PhageEditor editor;
    double binding_energy = 0.0;
    float binding_score = 0.0;
    bool show_binding_results = false;
    bool auto_update = true;
};

AppState g_state;

// GLFW error callback
void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// Update binding when genome changes
void onGenomeChanged() {
    if (g_state.auto_update) {
        physics::ElectrostaticsSolver solver;
        g_state.binding_energy = solver.computeBindingEnergy(
            g_state.phage_genome, 
            g_state.bacteria
        );
        g_state.binding_score = physics::BindingAssay::scoreBinding(g_state.binding_energy);
        g_state.show_binding_results = true;
    }
}

// Main GUI
void renderMainWindow() {
    ImGui::Begin("PhageForge - Main", nullptr, ImGuiWindowFlags_MenuBar);
    
    // Menu
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Auto-Update", nullptr, &g_state.auto_update);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    
    // Status bar
    ImGui::Text("Status: %s", g_state.show_binding_results ? "Binding calculated" : "Ready");
    ImGui::SameLine();
    ImGui::Text("| Score: %.1f/100", g_state.binding_score);
    
    ImGui::Separator();
    
    // Two-column layout
    ImGui::Columns(2, "MainColumns", true);
    
    // Left column: Phage Editor
    ImGui::Text("Phage Genome");
    ImGui::Separator();
    g_state.editor.render();
    
    ImGui::NextColumn();
    
    // Right column: Results
    ImGui::Text("Binding Results");
    ImGui::Separator();
    
    if (g_state.show_binding_results) {
        ImGui::Text("Binding Energy: %.2f kJ/mol", g_state.binding_energy);
        ImGui::Text("Binding Score: %.1f/100", g_state.binding_score);
        
        std::string desc = physics::BindingAssay::getBindingDescription(g_state.binding_energy);
        ImGui::Text("Description: %s", desc.c_str());
        
        double prob = physics::BindingAssay::infectionProbability(g_state.binding_energy);
        ImGui::Text("Infection Probability: %.1f%%", prob * 100.0);
        
        bool can_infect = physics::BindingAssay::isInfection(g_state.binding_energy);
        ImGui::Text("Infection: %s", can_infect ? "✅ Possible" : "❌ Not Possible");
        
        // Progress bar
        float progress = std::min(1.0f, g_state.binding_score / 100.0f);
        ImGui::ProgressBar(progress, ImVec2(-1, 20), 
            std::to_string(int(g_state.binding_score)).c_str());
        
        ImGui::Separator();
        
        // Bacteria info
        ImGui::Text("Bacteria: %s", g_state.bacteria.getName().c_str());
        ImGui::Text("Population: %.2f", g_state.bacteria.getPopulationDensity());
        ImGui::Text("Receptors: %zu", g_state.bacteria.getReceptors().size());
        
        if (ImGui::Button("Recalculate Binding")) {
            onGenomeChanged();
        }
    } else {
        ImGui::Text("Click 'Recalculate' or enable Auto-Update");
    }
    
    ImGui::Columns(1);
    ImGui::End();
}

int main() {
    std::cout << "=== PhageForge - GUI Version ===" << std::endl;
    std::cout << "Loading..." << std::endl;
    
    // Initialize amino acid properties
    try {
        biology::AminoAcidPropertiesManager::instance().loadFromTOML("config/amino_acids.toml");
        std::cout << "✅ Amino acid properties loaded" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "❌ Failed to load: " << e.what() << std::endl;
        return 1;
    }
    
    // Setup GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }
    
    // Create window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(1280, 720, "PhageForge", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return 1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    // Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
    
    // Initialize state
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    g_state.phage_genome = biology::GenomeFactory::createRandom(rng, 20);
    g_state.editor.setGenome(g_state.phage_genome);
    g_state.editor.setOnMutation(onGenomeChanged);
    
    // Add some bacteria receptors
    biology::Receptor r1;
    r1.setPosition({0.0, 0.0, 0.0});
    r1.setCharge(-1.5);
    r1.setType("LPS");
    g_state.bacteria.addReceptor(r1);
    
    biology::Receptor r2;
    r2.setPosition({1.0, 0.8, 0.0});
    r2.setCharge(-0.8);
    r2.setType("OmpF");
    g_state.bacteria.addReceptor(r2);
    
    g_state.bacteria.setName("E. coli");
    g_state.bacteria.setPopulationDensity(1.0);
    
    // Initial binding calculation
    onGenomeChanged();
    
    std::cout << "✅ GUI initialized" << std::endl;
    std::cout << "   Press ESC to exit" << std::endl;
    
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        renderMainWindow();
        
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
    }
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    
    std::cout << "=== Phase 4 Complete ===" << std::endl;
    
    return 0;
}
