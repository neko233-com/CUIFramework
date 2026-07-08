#pragma once

#include <cui/style.h>
#include <cui/layout_types.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace cui::ui {

class Widget {
public:
    virtual ~Widget() = default;
    virtual void render(class Builder& builder) = 0;
    virtual bool handle_event(const class InputEvent& event) { return false; }

    LayoutConfig layout;
    Color background = Colors::Transparent;
    CornerRadius corner_radius;
};

class Builder {
public:
    Builder(class Renderer& renderer, float screen_width, float screen_height);
    ~Builder();

    // Declarative API
    void begin_layout(FlexDirection direction = FlexDirection::Column,
                      float gap = 0.0f,
                      Padding padding = Padding{});

    void end_layout();

    void set_padding(Padding padding);
    void set_gap(float gap);
    void set_size(float width, float height);
    void set_background(Color color);
    void set_corner_radius(CornerRadius radius);

    bool box(const std::string& id = "");
    bool text(const std::string& content, float font_size = 16.0f, Color color = Colors::White);
    bool button(const std::string& label, Color background = Colors::Accent, Color text_color = Colors::White);
    bool checkbox(const std::string& label, bool* value);
    bool slider(const std::string& label, float* value, float min = 0.0f, float max = 1.0f);

    bool hovered() const;
    bool clicked() const;

    // Layout result
    const LayoutResult& layout_result() const { return result_; }

    // Render commands
    const std::vector<RenderCommand>& render_commands() const { return commands_; }

private:
    struct StackFrame {
        LayoutConfig config;
        std::string id;
        Rect bounds;
    };

    Renderer& renderer_;
    float screen_width_;
    float screen_height_;
    std::vector<StackFrame> stack_;
    LayoutResult result_;
    std::vector<RenderCommand> commands_;
    std::vector<Rect> bounds_stack_;
};

} // namespace cui::ui
