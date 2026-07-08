#include <cui/renderer.h>
#include <glad/glad.h>
#include <SDL.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>
#include <cstring>
#include <algorithm>

namespace cui {

// ============================================================================
// Shader sources
// ============================================================================

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

uniform sampler2D uTexture;
uniform int uUseTexture;

out vec4 FragColor;

void main() {
    if (uUseTexture == 1) {
        float alpha = texture(uTexture, vTexCoord).r;
        FragColor = vec4(vColor.rgb, vColor.a * alpha);
    } else {
        FragColor = vColor;
    }
}
)";

// ============================================================================
// Vertex structure
// ============================================================================

struct Vertex {
    float x, y;
    float r, g, b, a;
    float u, v;
};

// ============================================================================
// Renderer implementation
// ============================================================================

class RendererImpl {
public:
    bool init(void* window_handle) {
        auto load_func = [](const char* name) -> void* {
            return (void*)SDL_GL_GetProcAddress(name);
        };

        if (!gladLoadGL((GLADloadfunc)load_func)) {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        std::printf("OpenGL %s\n", glGetString(GL_VERSION));
        std::printf("Renderer: %s\n", glGetString(GL_RENDERER));

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

        projection_loc_ = glGetUniformLocation(shader_program_, "uProjection");
        texture_loc_ = glGetUniformLocation(shader_program_, "uTexture");
        use_texture_loc_ = glGetUniformLocation(shader_program_, "uUseTexture");

        // Setup VAO/VBO
        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        int vp[4];
        glGetIntegerv(GL_VIEWPORT, vp);
        screen_width_ = (float)vp[2];
        screen_height_ = (float)vp[3];

        // Create a simple 1x1 white texture for non-textured rendering
        unsigned char white_pixel = 255;
        glGenTextures(1, &white_texture_);
        glBindTexture(GL_TEXTURE_2D, white_texture_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1, 1, 0,
                     GL_RED, GL_UNSIGNED_BYTE, &white_pixel);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        return true;
    }

    void shutdown() {
        if (white_texture_) glDeleteTextures(1, &white_texture_);
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

        float proj[16] = {
            2.0f / screen_width_, 0, 0, 0,
            0, -2.0f / screen_height_, 0, 0,
            0, 0, -1, 0,
            -1, 1, 0, 1
        };
        glUniformMatrix4fv(projection_loc_, 1, GL_FALSE, proj);
        glUniform1i(use_texture_loc_, 0);

        vertices_.clear();
    }

    void end_frame() {
        flush();
    }

    void draw_rect(const Rect& rect, const Color& color, const CornerRadius& radius = CornerRadius{}) {
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
        draw_rect({rect.x, rect.y, rect.width, width}, color);
        draw_rect({rect.x, rect.bottom() - width, rect.width, width}, color);
        draw_rect({rect.x, rect.y, width, rect.height}, color);
        draw_rect({rect.right() - width, rect.y, width, rect.height}, color);
    }

    void draw_text(const std::string& text, float x, float y, float font_size, const Color& color) {
        // Simple text rendering: draw rectangles for each character
        // TODO: Implement proper font rendering with stb_truetype
        float char_width = font_size * 0.6f;
        float cursor_x = x;

        for (char c : text) {
            if (c == ' ') {
                cursor_x += char_width * 0.5f;
                continue;
            }

            // Draw a simple rectangle for each character
            Rect char_rect = {cursor_x, y, char_width, font_size};
            draw_rect(char_rect, color);
            cursor_x += char_width;
        }
    }

    void set_clip_rect(const Rect& rect) {
        flush();
        glEnable(GL_SCISSOR_TEST);
        float y = screen_height_ - rect.bottom();
        glScissor((GLint)rect.x, (GLint)y, (GLsizei)rect.width, (GLsizei)rect.height);
    }

    void clear_clip_rect() {
        flush();
        glDisable(GL_SCISSOR_TEST);
    }

    void flush() {
        if (vertices_.empty()) return;

        glUseProgram(shader_program_);
        glUniform1i(use_texture_loc_, 0);
        glBindTexture(GL_TEXTURE_2D, white_texture_);

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

    static constexpr size_t MAX_VERTICES = 16384;

    GLuint shader_program_ = 0;
    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    GLuint white_texture_ = 0;
    GLint projection_loc_ = -1;
    GLint texture_loc_ = -1;
    GLint use_texture_loc_ = -1;
    float screen_width_ = 1280;
    float screen_height_ = 720;

    std::vector<Vertex> vertices_;
};

// ============================================================================
// Public API
// ============================================================================

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
