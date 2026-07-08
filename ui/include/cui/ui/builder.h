#pragma once

#include <cui/style.h>
#include <cui/layout_types.h>
#include <cui/renderer.h>
#include <cui/input.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace cui::ui {

struct Style {
    Color background = Colors::Transparent;
    Color text_color = Colors::White;
    CornerRadius corner_radius;
    Padding padding;
    float gap = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
    FlexDirection direction = FlexDirection::Column;
    Alignment cross_alignment = Alignment::Stretch;
};

class Builder {
public:
    Builder(Renderer& renderer, Input& input, float screen_width, float screen_height);
    ~Builder();

    Builder(const Builder&) = delete;
    Builder& operator=(const Builder&) = delete;

    void begin(const std::string& id = "", FlexDirection direction = FlexDirection::Column, float gap = 0.0f, Padding padding = Padding{});
    void end();

    Builder& background(Color color);
    Builder& text_color(Color color);
    Builder& corner_radius(float radius);
    Builder& padding(Padding padding);
    Builder& gap(float gap);
    Builder& width(float w);
    Builder& height(float h);
    Builder& size(float w, float h);

    bool text(const std::string& content, float font_size = 16.0f);
    bool button(const std::string& label);
    bool checkbox(const std::string& label, bool* value);
    bool slider(const std::string& label, float* value, float min = 0.0f, float max = 1.0f);
    bool separator(float thickness = 1.0f);

    bool hovered() const;
    bool clicked() const;
    bool active() const;

    const std::vector<RenderCommand>& render_commands() const { return commands_; }
    void render_all(Renderer& renderer);

private:
    Rect calculate_element_bounds(float width, float height);
    void push_element_bounds(const Rect& bounds);

    Renderer& renderer_;
    Input& input_;
    float screen_width_;
    float screen_height_;
    std::vector<Rect> bounds_stack_;
    std::vector<RenderCommand> commands_;
    Style current_style_;
};

} // namespace cui::ui
