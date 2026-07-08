#include <cui/renderer.h>
#include <glad/glad.h>
#include <SDL.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>
#include <cstring>
#include <algorithm>

// stb_truetype implementation
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

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
    float x, y;         // Position
    float r, g, b, a;   // Color
    float u, v;         // Texture coordinates
};

// ============================================================================
// Font Atlas
// ============================================================================

struct FontAtlas {
    GLuint texture = 0;
    int width = 0;
    int height = 0;
    float glyph_width = 0;
    float glyph_height = 0;
    stbtt_packedchar chars[96]; // ASCII 32-127
    bool initialized = false;
};

static FontAtlas create_font_atlas(const unsigned char* ttf_data, int ttf_size, float font_size) {
    FontAtlas atlas;

    // Initialize stb_truetype
    stbtt_context* ctx = stbtt_InitContext(nullptr, nullptr);
    if (!ctx) {
        throw std::runtime_error("Failed to initialize stb_truetype");
    }

    stbtt_fontinfo font;
    if (!stbtt_InitFont(&font, ttf_data, stbtt_GetFontOffsetForIndex(ttf_data, 0))) {
        stbtt_FreeContext(ctx);
        throw std::runtime_error("Failed to initialize font");
    }

    // Create font atlas (256x256 for ASCII characters)
    atlas.width = 256;
    atlas.height = 256;

    unsigned char* bitmap = new unsigned char[atlas.width * atlas.height]();

    float scale = stbtt_ScaleForPixelHeight(&font, font_size);

    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);

    float baseline = (float)ascent * scale;

    // Pack characters into atlas
    int x = 1, y = 1;
    for (int i = 32; i < 128; i++) {
        int advance, leftBearing;
        stbtt_GetCodepointHMetrics(&font, i, &advance, &leftBearing);

        int ix0, iy0, ix1, iy1;
        stbtt_GetCodepointBitmapBox(&font, i, scale, scale, &ix0, &iy0, &ix1, &iy1);

        int char_width = ix1 - ix0;
        int char_height = iy1 - iy0;

        // Check if we need to wrap to next row
        if (x + char_width + 1 >= atlas.width) {
            x = 1;
            y += (int)font_size + 2;
        }

        // Get bitmap for this character
        int bx0, by0, bw, bh;
        unsigned char* char_bitmap = stbtt_GetCodepointBitmap(&font, i, scale, scale, &bw, &bh, &bx0, &by0);

        // Copy to atlas
        if (char_bitmap) {
            for (int row = 0; row < bh; row++) {
                for (int col = 0; col < bw; col++) {
                    int atlas_x = x + col;
                    int atlas_y = y + row;
                    if (atlas_x < atlas.width && atlas_y < atlas.height) {
                        atlas.bitmap[atlas_y * atlas.width + atlas_x] = char_bitmap[row * bw + col];
                    }
                }
            }
            stbtt_FreeBitmap(char_bitmap, nullptr);
        }

        // Store character metrics
        atlas.chars[i - 32].x0 = (float)x;
        atlas.chars[i - 32].y0 = (float)y;
        atlas.chars[i - 32].x1 = (float)(x + char_width);
        atlas.chars[i - 32].y1 = (float)(y + char_height);

        x += char_width + 1;
    }

    // Create OpenGL texture
    glGenTextures(1, &atlas.texture);
    glBindTexture(GL_TEXTURE_2D, atlas.texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas.width, atlas.height, 0,
                 GL_RED, GL_UNSIGNED_BYTE, atlas.bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    delete[] atlas.bitmap;
    atlas.initialized = true;

    stbtt_FreeContext(ctx);
    return atlas;
}

// ============================================================================
// Renderer implementation
// ============================================================================

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

        // Check OpenGL version
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

        // Get uniform locations
        projection_loc_ = glGetUniformLocation(shader_program_, "uProjection");
        texture_loc_ = glGetUniformLocation(shader_program_, "uTexture");
        use_texture_loc_ = glGetUniformLocation(shader_program_, "uUseTexture");

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

        // Enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Default viewport
        int vp[4];
        glGetIntegerv(GL_VIEWPORT, vp);
        screen_width_ = (float)vp[2];
        screen_height_ = (float)vp[3];

        // Create default font atlas
        create_default_font();

        return true;
    }

    void shutdown() {
        if (font_atlas_.texture) {
            glDeleteTextures(1, &font_atlas_.texture);
        }
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
        glUniform1i(use_texture_loc_, 0);

        vertices_.clear();
        text_vertices_.clear();
    }

    void end_frame() {
        // Draw opaque geometry first
        flush();

        // Draw text (with texture)
        flush_text();
    }

    void draw_rect(const Rect& rect, const Color& color, const CornerRadius& radius) {
        float r = color.rf(), g = color.gf(), b = color.bf(), a = color.af();

        // Two triangles for a quad
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
        if (!font_atlas_.initialized || text.empty()) return;

        float scale = font_size / 16.0f; // Scale relative to base font size
        float cursor_x = x;

        for (char c : text) {
            if (c < 32 || c >= 128) continue;

            const stbtt_packedchar& ch = font_atlas_.chars[c - 32];
            float char_w = (ch.x1 - ch.x0) * scale;
            float char_h = (ch.y1 - ch.y0) * scale;

            // Character UV coordinates
            float u0 = ch.x0 / (float)font_atlas_.width;
            float v0 = ch.y0 / (float)font_atlas_.height;
            float u1 = ch.x1 / (float)font_atlas_.width;
            float v1 = ch.y1 / (float)font_atlas_.height;

            float r = color.rf(), g = color.gf(), b = color.bf(), a = color.af();

            // Two triangles for character quad
            text_vertices_.push_back({cursor_x, y, r, g, b, a, u0, v0});
            text_vertices_.push_back({cursor_x + char_w, y, r, g, b, a, u1, v0});
            text_vertices_.push_back({cursor_x + char_w, y + char_h, r, g, b, a, u1, v1});

            text_vertices_.push_back({cursor_x, y, r, g, b, a, u0, v0});
            text_vertices_.push_back({cursor_x + char_w, y + char_h, r, g, b, a, u1, v1});
            text_vertices_.push_back({cursor_x, y + char_h, r, g, b, a, u0, v1});

            cursor_x += char_w;

            if (text_vertices_.size() >= MAX_VERTICES) flush_text();
        }
    }

    void set_clip_rect(const Rect& rect) {
        flush();
        flush_text();
        glEnable(GL_SCISSOR_TEST);
        float y = screen_height_ - rect.bottom();
        glScissor((GLint)rect.x, (GLint)y, (GLsizei)rect.width, (GLsizei)rect.height);
    }

    void clear_clip_rect() {
        flush();
        flush_text();
        glDisable(GL_SCISSOR_TEST);
    }

    void flush() {
        if (vertices_.empty()) return;

        glUseProgram(shader_program_);
        glUniform1i(use_texture_loc_, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

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

    void flush_text() {
        if (text_vertices_.empty()) return;

        glUseProgram(shader_program_);
        glUniform1i(use_texture_loc_, 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, font_atlas_.texture);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER,
                     text_vertices_.size() * sizeof(Vertex),
                     text_vertices_.data(),
                     GL_DYNAMIC_DRAW);

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)text_vertices_.size());
        text_vertices_.clear();
        glBindVertexArray(0);
    }

private:
    void create_default_font() {
        // Use a minimal embedded font (or load from file)
        // For now, create a simple 1x1 white pixel texture as placeholder
        unsigned char white_pixel = 255;
        glGenTextures(1, &font_atlas_.texture);
        glBindTexture(GL_TEXTURE_2D, font_atlas_.texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1, 1, 0,
                     GL_RED, GL_UNSIGNED_BYTE, &white_pixel);
        font_atlas_.width = 1;
        font_atlas_.height = 1;
        font_atlas_.initialized = true;
    }

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
    GLint projection_loc_ = -1;
    GLint texture_loc_ = -1;
    GLint use_texture_loc_ = -1;
    float screen_width_ = 1280;
    float screen_height_ = 720;

    std::vector<Vertex> vertices_;
    std::vector<Vertex> text_vertices_;

    FontAtlas font_atlas_;
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
