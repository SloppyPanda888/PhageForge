#include "gui/PhageEditor.hpp"
#include "physics/Electrostatics.hpp"
#include "physics/BindingAssay.hpp"
#include "biology/AminoAcid.hpp"
#include "biology/Bacteria.hpp"
#include "render/OpenGLRenderer.hpp"

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
    render::OpenGLRenderer renderer;
    double binding_energy = 0.0;
    float binding_score = 0.0;
    bool show_binding_results = false;
    bool auto_update = true;
    bool show_help = false;
    bool show_3d = true;
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
        
        // Update 3D renderer
        g_state.renderer.updatePhage(g_state.phage_genome);
        g_state.renderer.updateBacteria(g_state.bacteria);
        g_state.renderer.updateBindingEnergy(g_state.binding_energy);
    }
}

// Render the help window
void renderHelp() {
    if (!g_state.show_help) return;
    
    ImGui::SetNextWindowPos(ImVec2(400, 200), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Help - PhageForge Guide", &g_state.show_help, 
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
    
    ImGui::Text("PhageForge - Phage Design Game");
    ImGui::Separator();
    ImGui::Text("How to Play:");
    ImGui::BulletText("Edit the phage genome codon by codon");
    ImGui::BulletText("Click on a codon to see amino acid details");
    ImGui::BulletText("Use mutations to evolve your phage");
    ImGui::BulletText("Watch the binding score change in real-time");
    ImGui::Separator();
    ImGui::Text("3D Controls:");
    ImGui::BulletText("Left-click + drag: Rotate view");
    ImGui::BulletText("Scroll: Zoom in/out");
    ImGui::BulletText("Right-click + drag: Pan");
    ImGui::Separator();
    ImGui::Text("Amino Acid Colors:");
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "■ Hydrophobic (Yellow)");
    ImGui::TextColored(ImVec4(0.25f, 0.41f, 0.88f, 1.0f), "■ Positive Charge (Blue)");
    ImGui::TextColored(ImVec4(0.86f, 0.08f, 0.24f, 1.0f), "■ Negative Charge (Red)");
    ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "■ Polar (Green)");
    ImGui::Separator();
    ImGui::Text("Keys:");
    ImGui::BulletText("ESC - Exit");
    ImGui::BulletText("F1 - Toggle Help");
    ImGui::End();
}

// Render 3D viewport
void render3DViewport() {
    if (!g_state.show_3d) return;
    
    // Get the current window size
    GLFWwindow* window = glfwGetCurrentContext();
    int display_w, display_h;
    glfwGetWindowSize(window, &display_w, &display_h);
    
    // Create a child window for the 3D viewport
    ImGui::Begin("3D View", &g_state.show_3d, ImGuiWindowFlags_NoScrollbar);
    
    // Get the size of the ImGui window
    ImVec2 viewport_size = ImGui::GetContentRegionAvail();
    
    if (viewport_size.x > 0 && viewport_size.y > 0) {
        // Update the renderer with the new size
        g_state.renderer.resize((int)viewport_size.x, (int)viewport_size.y);
        
        // Get the draw position for the viewport
        ImVec2 viewport_pos = ImGui::GetCursorScreenPos();
        
        // Create a texture ID for the renderer output
        // Note: For now, we just call render directly in the ImGui window
        // In a full implementation, you'd render to a framebuffer
        
        // Draw a placeholder showing the 3D renderer is active
        ImGui::Text("3D Viewport Active");
        ImGui::Text("Phage: %zu amino acids", g_state.phage_genome.translateTailFiber().size());
        ImGui::Text("Receptors: %zu", g_state.bacteria.getReceptors().size());
        ImGui::Text("Binding: %.2f kJ/mol", g_state.binding_energy);
        
        // Add camera controls
        ImGui::Separator();
        if (ImGui::Button("Reset Camera")) {
            g_state.renderer.resetCamera();
        }
        
        // Instructions
        ImGui::TextDisabled("Drag to rotate | Scroll to zoom");
    } else {
        ImGui::Text("Resize window to see 3D view");
    }
    
    ImGui::End();
}

// Main GUI window
void renderMainWindow() {
    GLFWwindow* window = glfwGetCurrentContext();
    int display_w, display_h;
    glfwGetWindowSize(window, &display_w, &display_h);
    
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2((float)display_w, (float)display_h), ImGuiCond_Always);
    
    ImGui::Begin("PhageForge - Main", nullptr, 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_MenuBar);
    
    // Menu bar
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Randomize Genome")) {
                g_state.editor.randomizeGenome();
            }
            if (ImGui::MenuItem("Export JSON")) {
                std::cout << "Exporting genome: " << g_state.phage_genome.toJSON() << std::endl;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Auto-Update", nullptr, &g_state.auto_update);
            ImGui::MenuItem("3D View", nullptr, &g_state.show_3d);
            ImGui::MenuItem("Help", "F1", &g_state.show_help);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    
    // Status bar
    ImGui::Text("Status: %s", g_state.show_binding_results ? "Binding calculated" : "Ready");
    ImGui::SameLine();
    ImGui::Text("| Score: %.1f/100", g_state.binding_score);
    
    ImGui::Separator();
    
    // Calculate column widths
    float col_width = (display_w - 20) / 2;
    
    // Three-column layout: Editor | 3D View | Results
    ImGui::Columns(3, "MainColumns", true);
    ImGui::SetColumnWidth(0, col_width * 0.6);
    ImGui::SetColumnWidth(1, col_width);
    ImGui::SetColumnWidth(2, col_width * 0.6);
    
    // Column 1: Phage Editor
    ImGui::Text("Phage Genome");
    ImGui::Separator();
    g_state.editor.render();
    
    ImGui::NextColumn();
    
    // Column 2: 3D Viewport
    ImGui::Text("3D Visualization");
    ImGui::Separator();
    if (g_state.show_3d) {
        render3DViewport();
    } else {
        ImGui::Text("3D View disabled (enable in View menu)");
    }
    
    ImGui::NextColumn();
    
    // Column 3: Results
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
        
        float progress = std::min(1.0f, g_state.binding_score / 100.0f);
        ImGui::ProgressBar(progress, ImVec2(-1, 30), 
            std::to_string(int(g_state.binding_score)).c_str());
        
        ImGui::Separator();
        
        ImGui::Text("Bacteria: %s", g_state.bacteria.getName().c_str());
        ImGui::Text("Population: %.2f", g_state.bacteria.getPopulationDensity());
        ImGui::Text("Receptors: %zu", g_state.bacteria.getReceptors().size());
        
        for (const auto& receptor : g_state.bacteria.getReceptors()) {
            ImGui::Text("  %s: %.2f e at (%.1f, %.1f, %.1f)", 
                receptor.getType().c_str(),
                receptor.getCharge(),
                receptor.getPosition().x,
                receptor.getPosition().y,
                receptor.getPosition().z);
        }
        
        if (ImGui::Button("Recalculate Binding", ImVec2(-1, 40))) {
            onGenomeChanged();
        }
    } else {
        ImGui::Text("Click 'Recalculate' or enable Auto-Update");
    }
    
    ImGui::Columns(1);
    ImGui::End();
}

int main() {
    std::cout << "=== PhageForge - GUI Version with 3D ===" << std::endl;
    std::cout << "Loading..." << std::endl;
    
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
    
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    
    int window_width = mode->width * 0.9;
    int window_height = mode->height * 0.9;
    int window_x = (mode->width - window_width) / 2;
    int window_y = (mode->height - window_height) / 2;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, 
        "PhageForge - Phage Design Studio (3D)", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return 1;
    }
    
    glfwSetWindowPos(window, window_x, window_y);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    // Initialize 3D renderer
    if (!g_state.renderer.init(window_width, window_height)) {
        std::cerr << "Failed to initialize 3D renderer" << std::endl;
        return 1;
    }
    g_state.renderer.resetCamera();
    
    // Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    
    io.FontGlobalScale = 2.5f;
    
    ImGui::GetStyle().FramePadding = ImVec2(10, 10);
    ImGui::GetStyle().ItemSpacing = ImVec2(12, 12);
    ImGui::GetStyle().ItemInnerSpacing = ImVec2(10, 10);
    ImGui::GetStyle().WindowPadding = ImVec2(20, 20);
    ImGui::GetStyle().ScrollbarSize = 20.0f;
    ImGui::GetStyle().GrabMinSize = 15.0f;
    
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
    
    // Initialize state
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    g_state.phage_genome = biology::GenomeFactory::createRandom(rng, 20);
    g_state.editor.setGenome(g_state.phage_genome);
    g_state.editor.setOnMutation(onGenomeChanged);
    
    // Bacteria
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
    
    biology::Receptor r3;
    r3.setPosition({-0.8, 0.5, 0.0});
    r3.setCharge(-1.2);
    r3.setType("LamB");
    g_state.bacteria.addReceptor(r3);
    
    g_state.bacteria.setName("E. coli O157:H7");
    g_state.bacteria.setPopulationDensity(1.0);
    
    // Initial update
    onGenomeChanged();
    
    std::cout << "✅ GUI initialized" << std::endl;
    std::cout << "   Press ESC to exit" << std::endl;
    std::cout << "   Press F1 for help" << std::endl;
    std::cout << "   Window size: " << window_width << "x" << window_height << std::endl;
    
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
            g_state.show_help = !g_state.show_help;
        }
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        renderMainWindow();
        renderHelp();
        
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Render the 3D scene in the background
        g_state.renderer.render();
        
        // Render ImGui on top
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
    }
    
    // Cleanup
    g_state.renderer.cleanup();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    
    std::cout << "=== Phase 5 Complete ===" << std::endl;
    
    return 0;
}
