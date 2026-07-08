#include <cui/renderer.h>
#include <glad/glad.h>
#include <SDL.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>

namespace cui {

// Simple shader sources
static const char* vertex_shader_src = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 uProjection;

out vec4 vColor;
out vec2 vTexCoord;

void main() {
    gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
    vColor = aColor;
    vTexCoord = aTexCoord;
}
)";

static const char* fragment_shader_src = R"(
#version 330 core
in vec4 vColor;
in vec2 vTexCoord;

out vec4 FragColor;

void main() {
    FragColor = vColor;
}
)";

struct Vertex {
    float x, y;
    float r, g, b, a;
    float u, v;
};

static GLuint compile_shader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        throw std::runtime_error(std::string("Shader compilation failed: ") + log);
    }
    return shader;
}

class RendererImpl {
public:
    bool init(void* window_handle) {
        // Load OpenGL functions via GLAD
        auto load_func = [](const char* name) -> void* {
            return (void*)SDL_GL_GetProcAddress(name);
        };

        if (!gladLoadGL((GLADloadfunc)load_func)) {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        // Compile shaders
        GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_shader_src);
        GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_src);

        shader_program_ = glCreateProgram();
        glAttachShader(shader_program_, vs);
        glAttachShader(shader_program_, fs);
        glLinkProgram(shader_program_);

        GLint success;
        glGetProgramiv(shader_program_, GL_LINK_STATUS, &success);
        if (!success) {
            char log[512];
            glGetProgramInfoLog(shader_program_, 512, nullptr, log);
            throw std::runtime_error(std::string("Shader link failed: ") + log);
        }

        glDeleteShader(vs);
        glDeleteShader(fs);

        // Setup VAO/VBO
        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);

        // Position (location 0)
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        // Color (location 1)
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // TexCoord (location 2)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        // Setup projection uniform
        projection_loc_ = glGetUniformLocation(shader_program_, "uProjection");

        // Enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Default viewport
        int vp[4];
        glGetIntegerv(GL_VIEWPORT, vp);
        screen_width_ = (float)vp[2];
        screen_height_ = (float)vp[3];

        return true;
    }

    void shutdown() {
        if (vbo_) glDeleteBuffers(1, &vbo_);
        if (vao_) glDeleteVertexArrays(1, &vao_);
        if (shader_program_) glDeleteProgram(shader_program_);
    }

    void set_viewport(int x, int y, int w, int h) {
        glViewport(x, y, w, h);
        screen_width_ = (float)w;
        screen_height_ = (float)h;
    }

    void begin_frame() {
        glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program_);

        // Orthographic projection: top-left origin
        float proj[16] = {
            2.0f / screen_width_, 0, 0, 0,
            0, -2.0f / screen_height_, 0, 0,
            0, 0, -1, 0,
            -1, 1, 0, 1
        };
        glUniformMatrix4fv(projection_loc_, 1, GL_FALSE, proj);

        vertices_.clear();
    }

    void end_frame() {
        flush();
    }

    void draw_rect(const Rect& rect, const Color& color, const CornerRadius& radius) {
        // For simplicity, draw as a quad (corner radius TODO: use stencil or fragment shader)
        float r = color.rf(), g = color.gf(), b = color.bf(), a = color.af();
        vertices_.push_back({rect.x, rect.y, r, g, b, a, 0, 0});
        vertices_.push_back({rect.right(), rect.y, r, g, b, a, 1, 0});
        vertices_.push_back({rect.right(), rect.bottom(), r, g, b, a, 1, 1});
        vertices_.push_back({rect.x, rect.y, r, g, b, a, 0, 0});
        vertices_.push_back({rect.right(), rect.bottom(), r, g, b, a, 1, 1});
        vertices_.push_back({rect.x, rect.bottom(), r, g, b, a, 0, 1});

        if (vertices_.size() >= MAX_VERTICES) flush();
    }

    void draw_border(const Rect& rect, const Color& color, float width, const CornerRadius& radius) {
        float r = color.rf(), g = color.gf(), b = color.bf(), a = color.af();
        // Top
        draw_rect({rect.x, rect.y, rect.width, width}, color);
        // Bottom
        draw_rect({rect.x, rect.bottom() - width, rect.width, width}, color);
        // Left
        draw_rect({rect.x, rect.y, width, rect.height}, color);
        // Right
        draw_rect({rect.right() - width, rect.y, width, rect.height}, color);
    }

    void draw_text(const std::string& text, float x, float y, float font_size, const Color& color) {
        // TODO: Implement text rendering with stb_truetype
        // For now, just draw a placeholder rect
        draw_rect({x, y, (float)text.length() * font_size * 0.6f, font_size}, color);
    }

    void set_clip_rect(const Rect& rect) {
        flush();
        glEnable(GL_SCISSOR_TEST);
        // Convert to OpenGL clip coordinates (bottom-left origin)
        float y = screen_height_ - rect.bottom();
        glScissor((GLint)rect.x, (GLint)y, (GLsizei)rect.width, (GLsizei)rect.height);
    }

    void clear_clip_rect() {
        flush();
        glDisable(GL_SCISSOR_TEST);
    }

    void flush() {
        if (vertices_.empty()) return;

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER,
                     vertices_.size() * sizeof(Vertex),
                     vertices_.data(),
                     GL_DYNAMIC_DRAW);

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices_.size());
        vertices_.clear();
        glBindVertexArray(0);
    }

private:
    static constexpr size_t MAX_VERTICES = 16384;

    GLuint shader_program_ = 0;
    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    GLint projection_loc_ = -1;
    float screen_width_ = 1280;
    float screen_height_ = 720;
    std::vector<Vertex> vertices_;
};

Renderer::Renderer() : impl_(std::make_unique<RendererImpl>()) {}
Renderer::~Renderer() { if (impl_) shutdown(); }

bool Renderer::init(void* window_handle) { return impl_->init(window_handle); }
void Renderer::shutdown() { impl_->shutdown(); }
void Renderer::begin_frame() { impl_->begin_frame(); }
void Renderer::end_frame() { impl_->end_frame(); }
void Renderer::set_viewport(int x, int y, int w, int h) { impl_->set_viewport(x, y, w, h); }
void Renderer::draw_rect(const Rect& rect, const Color& color, const CornerRadius& radius) { impl_->draw_rect(rect, color, radius); }
void Renderer::draw_border(const Rect& rect, const Color& color, float width, const CornerRadius& radius) { impl_->draw_border(rect, color, width, radius); }
void Renderer::draw_text(const std::string& text, float x, float y, float font_size, const Color& color) { impl_->draw_text(text, x, y, font_size, color); }
void Renderer::set_clip_rect(const Rect& rect) { impl_->set_clip_rect(rect); }
void Renderer::clear_clip_rect() { impl_->clear_clip_rect(); }
void Renderer::flush() { impl_->flush(); }

} // namespace cui
