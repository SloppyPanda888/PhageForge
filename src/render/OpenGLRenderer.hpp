#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../biology/Genome.hpp"
#include "../biology/Bacteria.hpp"
#include "../physics/Electrostatics.hpp"

#include <vector>
#include <memory>

namespace phageforge::render {

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    float charge;
};

class OpenGLRenderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer();

    bool init(int width, int height);
    void cleanup();
    
    void resize(int width, int height);
    
    // Update scene data
    void updatePhage(const biology::Genome& genome);
    void updateBacteria(const biology::BacterialStrain& bacteria);
    void updateBindingEnergy(double energy);
    
    // Render the scene
    void render();
    
    // Camera controls
    void rotateCamera(float dx, float dy);
    void zoomCamera(float delta);
    void resetCamera();
    
    // Get current camera position
    glm::vec3 getCameraPosition() const { return m_camera_pos; }
    
private:
    // Shader programs
    GLuint m_shader_program;
    GLuint m_vertex_shader;
    GLuint m_fragment_shader;
    
    // Buffers
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    
    // Scene data
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    
    // Scene objects
    std::vector<glm::vec3> m_phage_positions;
    std::vector<glm::vec3> m_phage_colors;
    std::vector<glm::vec3> m_receptor_positions;
    std::vector<glm::vec3> m_receptor_colors;
    std::vector<std::pair<glm::vec3, glm::vec3>> m_binding_lines;
    std::vector<float> m_binding_energies;
    
    // Camera
    glm::vec3 m_camera_pos;
    glm::vec3 m_camera_target;
    glm::vec3 m_camera_up;
    glm::mat4 m_view;
    glm::mat4 m_projection;
    
    float m_camera_radius;
    float m_camera_theta;
    float m_camera_phi;
    
    int m_width;
    int m_height;
    
    // Scene state
    bool m_phage_updated;
    bool m_bacteria_updated;
    double m_binding_energy;
    
    // Helper methods
    bool compileShaders();
    void setupBuffers();
    void buildPhageGeometry(const biology::Genome& genome);
    void buildBacteriaGeometry(const biology::BacterialStrain& bacteria);
    void buildBindingLines(const physics::ElectrostaticsSolver& solver,
                          const biology::Genome& phage,
                          const biology::BacterialStrain& bacteria);
    void updateCamera();
    
    // Generate sphere vertices
    std::vector<Vertex> generateSphere(const glm::vec3& center, 
                                        float radius, 
                                        const glm::vec3& color,
                                        int segments = 16);
};

} // namespace phageforge::render
