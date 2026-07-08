#pragma once

#include <cui/style.h>
#include <cui/layout_types.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <type_traits>

namespace cui::ui {

// ============================================================================
// Style properties
// ============================================================================

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

// ============================================================================
// Builder — Declarative UI API
// ============================================================================

class Builder {
public:
    Builder(class Renderer& renderer, float screen_width, float screen_height);
    ~Builder();

    // Non-copyable, movable
    Builder(const Builder&) = delete;
    Builder& operator=(const Builder&) = delete;
    Builder(Builder&&) = default;
    Builder& operator=(Builder&&) = default;

    // ---- Layout primitives ----
    void begin(const std::string& id = "", FlexDirection direction = FlexDirection::Column, float gap = 0.0f, Padding padding = Padding{});
    void end();

    // ---- Style setters (apply to next element) ----
    Builder& background(Color color);
    Builder& text_color(Color color);
    Builder& corner_radius(float radius);
    Builder& padding(Padding padding);
    Builder& gap(float gap);
    Builder& width(float w);
    Builder& height(float h);
    Builder& size(float w, float h);

    // ---- Elements ----
    bool text(const std::string& content, float font_size = 16.0f);
    bool button(const std::string& label);
    bool checkbox(const std::string& label, bool* value);
    bool slider(const std::string& label, float* value, float min = 0.0f, float max = 1.0f);
    bool separator(float thickness = 1.0f);

    // ---- Interaction state ----
    bool hovered() const;
    bool clicked() const;
    bool active() const;

    // ---- Render commands ----
    const std::vector<RenderCommand>& render_commands() const { return commands_; }
    void render_all(class Renderer& renderer);

    // ---- Layout result ----
    const LayoutResult& layout_result() const { return result_; }

private:
    struct ElementState {
        bool hovered = false;
        bool clicked = false;
        bool active = false;
    };

    struct StackFrame {
        std::string id;
        LayoutConfig config;
        Rect bounds;
        std::vector<Rect> children_bounds;
    };

    Rect calculate_element_bounds(const std::string& id, float width, float height);
    void push_element_bounds(const Rect& bounds);
    ElementState get_element_state(const Rect& bounds) const;

    Renderer& renderer_;
    float screen_width_;
    float screen_height_;
    std::vector<StackFrame> stack_;
    std::vector<Rect> bounds_stack_;
    std::vector<RenderCommand> commands_;
    std::vector<ElementState> element_states_;
    LayoutResult result_;
    Style current_style_;
};

// ============================================================================
// Helper functions for creating styles
// ============================================================================

inline Style make_style() { return Style{}; }

template<typename... Args>
Style make_style(Args&&... args) {
    Style s;
    // Apply each argument using perfect forwarding
    // This is a simplified version; in production you'd use a more robust approach
    return s;
}

} // namespace cui::ui
