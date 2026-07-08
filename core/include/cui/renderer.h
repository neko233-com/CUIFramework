#pragma once

#include <cui/style.h>
#include <cui/layout_types.h>
#include <vector>
#include <cstdint>

namespace cui {

class RendererImpl;

class Renderer {
public:
    Renderer();
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    bool init(void* window_handle);
    void shutdown();

    void begin_frame();
    void end_frame();

    void set_viewport(int x, int y, int w, int h);
    void draw_rect(const Rect& rect, const Color& color, const CornerRadius& radius = {});
    void draw_border(const Rect& rect, const Color& color, float width, const CornerRadius& radius = {});
    void draw_text(const std::string& text, float x, float y, float font_size, const Color& color);
    void set_clip_rect(const Rect& rect);
    void clear_clip_rect();

    // Batch rendering
    void flush();

private:
    std::unique_ptr<RendererImpl> impl_;
};

} // namespace cui
