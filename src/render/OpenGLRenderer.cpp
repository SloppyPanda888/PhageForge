#include "OpenGLRenderer.hpp"
#include "../physics/VectorMath.hpp"
#include <iostream>
#include <cmath>

namespace phageforge::render {

OpenGLRenderer::OpenGLRenderer()
    : m_shader_program(0)
    , m_vertex_shader(0)
    , m_fragment_shader(0)
    , m_vao(0)
    , m_vbo(0)
    , m_ebo(0)
    , m_camera_pos(0.0f, 3.0f, 8.0f)
    , m_camera_target(0.0f, 0.0f, 0.0f)
    , m_camera_up(0.0f, 1.0f, 0.0f)
    , m_camera_radius(8.0f)
    , m_camera_theta(0.0f)
    , m_camera_phi(0.5f)
    , m_width(800)
    , m_height(600)
    , m_phage_updated(false)
    , m_bacteria_updated(false)
    , m_binding_energy(0.0)
{
    resetCamera();
}

OpenGLRenderer::~OpenGLRenderer() {
    cleanup();
}

bool OpenGLRenderer::init(int width, int height) {
    m_width = width;
    m_height = height;
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }
    
    // Compile shaders
    if (!compileShaders()) {
        return false;
    }
    
    // Setup buffers
    setupBuffers();
    
    // Set projection
    updateCamera();
    
    std::cout << "✅ 3D Renderer initialized" << std::endl;
    return true;
}

void OpenGLRenderer::cleanup() {
    if (m_shader_program) glDeleteProgram(m_shader_program);
    if (m_vertex_shader) glDeleteShader(m_vertex_shader);
    if (m_fragment_shader) glDeleteShader(m_fragment_shader);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
}

bool OpenGLRenderer::compileShaders() {
    // Vertex shader
    const char* vertex_source = R"(
        #version 150 core
        in vec3 aPos;
        in vec3 aColor;
        in float aCharge;
        
        uniform mat4 uView;
        uniform mat4 uProjection;
        
        out vec3 vColor;
        out float vCharge;
        
        void main() {
            gl_Position = uProjection * uView * vec4(aPos, 1.0);
            vColor = aColor;
            vCharge = aCharge;
        }
    )";
    
    m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(m_vertex_shader);
    
    // Check vertex shader
    GLint success;
    glGetShaderiv(m_vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(m_vertex_shader, 512, NULL, infoLog);
        std::cerr << "Vertex shader error: " << infoLog << std::endl;
        return false;
    }
    
    // Fragment shader
    const char* fragment_source = R"(
        #version 150 core
        in vec3 vColor;
        in float vCharge;
        
        out vec4 FragColor;
        
        void main() {
            // Add some glow effect based on charge
            vec3 color = vColor;
            if (vCharge > 0.5) {
                color += vec3(0.2, 0.2, 0.5);
            } else if (vCharge < -0.5) {
                color += vec3(0.5, 0.2, 0.2);
            }
            FragColor = vec4(color, 1.0);
        }
    )";
    
    m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(m_fragment_shader);
    
    // Check fragment shader
    glGetShaderiv(m_fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(m_fragment_shader, 512, NULL, infoLog);
        std::cerr << "Fragment shader error: " << infoLog << std::endl;
        return false;
    }
    
    // Link program
    m_shader_program = glCreateProgram();
    glAttachShader(m_shader_program, m_vertex_shader);
    glAttachShader(m_shader_program, m_fragment_shader);
    glLinkProgram(m_shader_program);
    
    // Check program
    glGetProgramiv(m_shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_shader_program, 512, NULL, infoLog);
        std::cerr << "Shader program error: " << infoLog << std::endl;
        return false;
    }
    
    return true;
}

void OpenGLRenderer::setupBuffers() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    
    glBindVertexArray(m_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    
    // Charge
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, charge));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
}

void OpenGLRenderer::resize(int width, int height) {
    m_width = width;
    m_height = height;
    updateCamera();
}

void OpenGLRenderer::updateCamera() {
    // Calculate camera position from spherical coordinates
    float x = m_camera_radius * sin(m_camera_phi) * cos(m_camera_theta);
    float y = m_camera_radius * cos(m_camera_phi);
    float z = m_camera_radius * sin(m_camera_phi) * sin(m_camera_theta);
    
    m_camera_pos = glm::vec3(x, y, z);
    m_view = glm::lookAt(m_camera_pos, m_camera_target, m_camera_up);
    
    float aspect = (m_height > 0) ? (float)m_width / (float)m_height : 1.0f;
    m_projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

void OpenGLRenderer::rotateCamera(float dx, float dy) {
    m_camera_theta += dx * 0.01f;
    m_camera_phi += dy * 0.01f;
    m_camera_phi = std::max(0.1f, std::min(3.04f, m_camera_phi));
    updateCamera();
}

void OpenGLRenderer::zoomCamera(float delta) {
    m_camera_radius = std::max(2.0f, std::min(20.0f, m_camera_radius + delta));
    updateCamera();
}

void OpenGLRenderer::resetCamera() {
    m_camera_radius = 8.0f;
    m_camera_theta = 0.0f;
    m_camera_phi = 0.5f;
    updateCamera();
}

void OpenGLRenderer::updatePhage(const biology::Genome& genome) {
    m_phage_updated = true;
    buildPhageGeometry(genome);
}

void OpenGLRenderer::updateBacteria(const biology::BacterialStrain& bacteria) {
    m_bacteria_updated = true;
    buildBacteriaGeometry(bacteria);
}

void OpenGLRenderer::updateBindingEnergy(double energy) {
    m_binding_energy = energy;
}

std::vector<Vertex> OpenGLRenderer::generateSphere(const glm::vec3& center, 
                                                     float radius, 
                                                     const glm::vec3& color,
                                                     int segments) {
    std::vector<Vertex> vertices;
    
    // Simplified sphere using triangle fan
    for (int i = 0; i <= segments; ++i) {
        float theta = (float)i / segments * 3.14159f;
        for (int j = 0; j <= segments; ++j) {
            float phi = (float)j / segments * 2.0f * 3.14159f;
            
            float x = center.x + radius * sin(theta) * cos(phi);
            float y = center.y + radius * cos(theta);
            float z = center.z + radius * sin(theta) * sin(phi);
            
            vertices.push_back({glm::vec3(x, y, z), color, 0.0f});
        }
    }
    
    return vertices;
}

void OpenGLRenderer::buildPhageGeometry(const biology::Genome& genome) {
    m_phage_positions.clear();
    m_phage_colors.clear();
    
    auto aa_sequence = genome.translateTailFiber();
    if (aa_sequence.empty()) return;
    
    // Build helix for phage tail
    const int num_aa = std::min(aa_sequence.size(), size_t(20));
    const float helix_radius = 0.5f;
    const float helix_pitch = 0.8f;
    const float sphere_radius = 0.15f;
    
    auto& aa_manager = biology::AminoAcidPropertiesManager::instance();
    
    for (int i = 0; i < num_aa; ++i) {
        auto aa = aa_sequence[i];
        glm::vec3 color(0.5f, 0.5f, 0.5f);
        float charge = 0.0f;
        
        try {
            auto props = aa_manager.getProperties(aa);
            charge = props.net_charge_at_ph7;
            
            // Color based on charge and hydrophobicity
            if (charge > 0.5f) {
                color = glm::vec3(0.2f, 0.3f, 1.0f);  // Blue (positive)
            } else if (charge < -0.5f) {
                color = glm::vec3(1.0f, 0.2f, 0.2f);  // Red (negative)
            } else if (props.hydrophobicity > 0.0f) {
                color = glm::vec3(1.0f, 0.8f, 0.0f);  // Yellow (hydrophobic)
            } else {
                color = glm::vec3(0.2f, 0.8f, 0.2f);  // Green (hydrophilic)
            }
        } catch (...) {
            color = glm::vec3(0.5f, 0.5f, 0.5f);
        }
        
        // Position along helix
        float t = (float)i / std::max(1, num_aa);
        float angle = t * 2.0f * 3.14159f * 3.0f;
        float z = t * helix_pitch * 10.0f;
        
        glm::vec3 pos(helix_radius * cos(angle), helix_radius * sin(angle), z);
        m_phage_positions.push_back(pos);
        m_phage_colors.push_back(color);
    }
}

void OpenGLRenderer::buildBacteriaGeometry(const biology::BacterialStrain& bacteria) {
    m_receptor_positions.clear();
    m_receptor_colors.clear();
    
    const auto& receptors = bacteria.getReceptors();
    for (const auto& receptor : receptors) {
        core::Point3D pos = receptor.getPosition();
        float charge = receptor.getCharge();
        
        glm::vec3 color;
        if (charge > 0.5f) {
            color = glm::vec3(0.2f, 0.5f, 1.0f);
        } else if (charge < -0.5f) {
            color = glm::vec3(1.0f, 0.3f, 0.3f);
        } else {
            color = glm::vec3(0.3f, 1.0f, 0.3f);
        }
        
        m_receptor_positions.push_back(glm::vec3(pos.x, pos.y, pos.z));
        m_receptor_colors.push_back(color);
    }
}

void OpenGLRenderer::buildBindingLines(const physics::ElectrostaticsSolver& solver,
                                       const biology::Genome& phage,
                                       const biology::BacterialStrain& bacteria) {
    m_binding_lines.clear();
    m_binding_energies.clear();
    
    auto interactions = solver.getInteractionMap(phage, bacteria);
    
    for (const auto& [aa_idx, receptor_idx, energy] : interactions) {
        if (aa_idx < m_phage_positions.size() && receptor_idx < m_receptor_positions.size()) {
            m_binding_lines.push_back({
                m_phage_positions[aa_idx],
                m_receptor_positions[receptor_idx]
            });
            m_binding_energies.push_back(energy);
        }
    }
}

void OpenGLRenderer::render() {
    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Use shader
    glUseProgram(m_shader_program);
    
    // Set uniforms
    GLint viewLoc = glGetUniformLocation(m_shader_program, "uView");
    GLint projLoc = glGetUniformLocation(m_shader_program, "uProjection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_projection));
    
    // Draw phage tail fiber as spheres (simplified as points for now)
    glPointSize(8.0f);
    
    // Draw phage positions
    if (!m_phage_positions.empty()) {
        glBindVertexArray(m_vao);
        
        // Create temporary vertices for phage
        std::vector<Vertex> phage_vertices;
        phage_vertices.reserve(m_phage_positions.size());
        for (size_t i = 0; i < m_phage_positions.size(); ++i) {
            phage_vertices.push_back({
                m_phage_positions[i],
                m_phage_colors[i],
                0.0f
            });
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, phage_vertices.size() * sizeof(Vertex), 
                     phage_vertices.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, phage_vertices.size());
    }
    
    // Draw bacterial receptors
    if (!m_receptor_positions.empty()) {
        std::vector<Vertex> receptor_vertices;
        receptor_vertices.reserve(m_receptor_positions.size());
        for (size_t i = 0; i < m_receptor_positions.size(); ++i) {
            receptor_vertices.push_back({
                m_receptor_positions[i],
                m_receptor_colors[i],
                0.0f
            });
        }
        
        glPointSize(12.0f);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, receptor_vertices.size() * sizeof(Vertex), 
                     receptor_vertices.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, receptor_vertices.size());
        glPointSize(8.0f);
    }
    
    // Draw binding lines
    if (!m_binding_lines.empty()) {
        glDisable(GL_DEPTH_TEST);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        for (const auto& [start, end] : m_binding_lines) {
            glColor3f(0.5f, 0.5f, 1.0f);
            glVertex3f(start.x, start.y, start.z);
            glColor3f(0.3f, 0.3f, 0.8f);
            glVertex3f(end.x, end.y, end.z);
        }
        glEnd();
        glEnable(GL_DEPTH_TEST);
    }
    
    glUseProgram(0);
    glBindVertexArray(0);
}

} // namespace phageforge::render
