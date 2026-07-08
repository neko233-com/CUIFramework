#include <cui/ui/builder.h>
#include <cui/renderer.h>
#include <algorithm>
#include <cmath>

namespace cui::ui {

Builder::Builder(Renderer& renderer, float screen_width, float screen_height)
    : renderer_(renderer)
    , screen_width_(screen_width)
    , screen_height_(screen_height)
{
    bounds_stack_.push_back({0, 0, screen_width, screen_height});
}

Builder::~Builder() = default;

// ============================================================================
// Layout primitives
// ============================================================================

void Builder::begin(const std::string& id, FlexDirection direction, float gap, Padding padding) {
    StackFrame frame;
    frame.id = id;
    frame.config.direction = direction;
    frame.config.gap = gap;
    frame.config.padding = padding;
    frame.bounds = bounds_stack_.back();
    stack_.push_back(std::move(frame));
}

void Builder::end() {
    if (!stack_.empty()) {
        auto& frame = stack_.back();

        // Update parent bounds based on children
        if (!bounds_stack_.empty() && !frame.children_bounds.empty()) {
            auto& parent_bounds = bounds_stack_.back();
            float max_x = parent_bounds.x;
            float max_y = parent_bounds.y;

            for (const auto& child : frame.children_bounds) {
                max_x = std::max(max_x, child.right());
                max_y = std::max(max_y, child.bottom());
            }

            // Update parent size to fit children (if using FitContent sizing)
            if (frame.config.widthSizing == Sizing::FitContent) {
                parent_bounds.width = max_x - parent_bounds.x + frame.config.padding.right;
            }
            if (frame.config.heightSizing == Sizing::FitContent) {
                parent_bounds.height = max_y - parent_bounds.y + frame.config.padding.bottom;
            }
        }

        stack_.pop_back();
    }

    if (!bounds_stack_.empty()) {
        bounds_stack_.pop_back();
    }
}

// ============================================================================
// Style setters
// ============================================================================

Builder& Builder::background(Color color) {
    current_style_.background = color;
    return *this;
}

Builder& Builder::text_color(Color color) {
    current_style_.text_color = color;
    return *this;
}

Builder& Builder::corner_radius(float radius) {
    current_style_.corner_radius = CornerRadius(radius);
    return *this;
}

Builder& Builder::padding(Padding padding) {
    current_style_.padding = padding;
    return *this;
}

Builder& Builder::gap(float gap) {
    current_style_.gap = gap;
    return *this;
}

Builder& Builder::width(float w) {
    current_style_.width = w;
    return *this;
}

Builder& Builder::height(float h) {
    current_style_.height = h;
    return *this;
}

Builder& Builder::size(float w, float h) {
    current_style_.width = w;
    current_style_.height = h;
    return *this;
}

// ============================================================================
// Elements
// ============================================================================

Rect Builder::calculate_element_bounds(const std::string& id, float width, float height) {
    if (stack_.empty()) {
        return {0, 0, width, height};
    }

    auto& frame = stack_.back();
    auto& parent = bounds_stack_.back();

    Rect bounds;
    float offset = 0;

    // Calculate offset based on existing children
    if (!frame.children_bounds.empty()) {
        const auto& last = frame.children_bounds.back();
        if (frame.config.direction == FlexDirection::Column) {
            offset = last.bottom() - parent.y + frame.config.gap;
        } else {
            offset = last.right() - parent.x + frame.config.gap;
        }
    }

    if (frame.config.direction == FlexDirection::Column) {
        bounds.x = parent.x + frame.config.padding.left;
        bounds.y = parent.y + frame.config.padding.top + offset;
        bounds.width = width > 0 ? width : (parent.width - frame.config.padding.left - frame.config.padding.right);
        bounds.height = height > 0 ? height : 30.0f; // Default height
    } else {
        bounds.x = parent.x + frame.config.padding.left + offset;
        bounds.y = parent.y + frame.config.padding.top;
        bounds.width = width > 0 ? width : 100.0f; // Default width
        bounds.height = height > 0 ? height : (parent.height - frame.config.padding.top - frame.config.padding.bottom);
    }

    return bounds;
}

void Builder::push_element_bounds(const Rect& bounds) {
    if (!stack_.empty()) {
        stack_.back().children_bounds.push_back(bounds);
    }
    bounds_stack_.push_back(bounds);
}

Builder::ElementState Builder::get_element_state(const Rect& bounds) const {
    ElementState state;
    auto mouse = renderer_.get_input().mouse_position();
    state.hovered = bounds.contains(mouse);
    state.clicked = state.hovered && renderer_.get_input().mouse_button_pressed(MouseButton::Left);
    state.active = state.hovered && renderer_.get_input().mouse_button_down(MouseButton::Left);
    return state;
}

bool Builder::text(const std::string& content, float font_size) {
    auto bounds = calculate_element_bounds("", 0, font_size);

    // Measure text width
    float text_width = content.length() * font_size * 0.6f;
    bounds.width = text_width;

    push_element_bounds(bounds);

    RenderCommand cmd;
    cmd.type = RenderCommand::Type::Text;
    cmd.bounds = bounds;
    cmd.text = content;
    cmd.fontSize = font_size;
    cmd.textColor = current_style_.text_color;
    commands_.push_back(cmd);

    // Reset style for next element
    current_style_ = Style{};

    return get_element_state(bounds).clicked;
}

bool Builder::button(const std::string& label) {
    auto bounds = calculate_element_bounds("", 120, 32);
    auto state = get_element_state(bounds);

    // Button background
    Color bg = state.active ? Colors::Accent : (state.hovered ? Color{80, 130, 220, 255} : Colors::DarkGray);

    RenderCommand cmd;
    cmd.type = RenderCommand::Type::Rect;
    cmd.bounds = bounds;
    cmd.color = bg;
    cmd.cornerRadius = CornerRadius(4.0f);
    commands_.push_back(cmd);

    // Label text centered in button
    RenderCommand text_cmd;
    text_cmd.type = RenderCommand::Type::Text;
    text_cmd.bounds = {bounds.x + 8, bounds.y + 8, bounds.width - 16, bounds.height - 16};
    text_cmd.text = label;
    text_cmd.fontSize = 14.0f;
    text_cmd.textColor = Colors::White;
    commands_.push_back(text_cmd);

    push_element_bounds(bounds);

    // Reset style
    current_style_ = Style{};

    return state.clicked;
}

bool Builder::checkbox(const std::string& label, bool* value) {
    auto bounds = calculate_element_bounds("", 200, 24);
    auto state = get_element_state(bounds);

    float box_size = 16.0f;
    Rect check_bounds;
    check_bounds.x = bounds.x;
    check_bounds.y = bounds.y + 4;
    check_bounds.width = box_size;
    check_bounds.height = box_size;

    // Box background
    RenderCommand cmd;
    cmd.type = RenderCommand::Type::Rect;
    cmd.bounds = check_bounds;
    cmd.color = *value ? Colors::Accent : Colors::DarkGray;
    cmd.cornerRadius = CornerRadius(3.0f);
    commands_.push_back(cmd);

    // Checkmark (if checked)
    if (*value) {
        RenderCommand check_cmd;
        check_cmd.type = RenderCommand::Type::Rect;
        check_cmd.bounds = {check_bounds.x + 3, check_bounds.y + 3, box_size - 6, box_size - 6};
        check_cmd.color = Colors::White;
        commands_.push_back(check_cmd);
    }

    // Label
    RenderCommand text_cmd;
    text_cmd.type = RenderCommand::Type::Text;
    text_cmd.bounds = {check_bounds.right() + 8, bounds.y, 150, bounds.height};
    text_cmd.text = label;
    text_cmd.fontSize = 14.0f;
    text_cmd.textColor = Colors::White;
    commands_.push_back(text_cmd);

    push_element_bounds(bounds);

    // Toggle on click
    if (state.clicked) {
        *value = !(*value);
    }

    // Reset style
    current_style_ = Style{};

    return state.clicked;
}

bool Builder::slider(const std::string& label, float* value, float min, float max) {
    auto bounds = calculate_element_bounds("", 200, 32);
    auto state = get_element_state(bounds);

    float slider_width = 150.0f;
    float slider_height = 6.0f;

    Rect slider_bounds;
    slider_bounds.x = bounds.x;
    slider_bounds.y = bounds.y + 13;
    slider_bounds.width = slider_width;
    slider_bounds.height = slider_height;

    // Track
    RenderCommand track_cmd;
    track_cmd.type = RenderCommand::Type::Rect;
    track_cmd.bounds = slider_bounds;
    track_cmd.color = Colors::DarkGray;
    track_cmd.cornerRadius = CornerRadius(3.0f);
    commands_.push_back(track_cmd);

    // Fill
    float t = std::clamp((*value - min) / (max - min), 0.0f, 1.0f);
    Rect fill_bounds = slider_bounds;
    fill_bounds.width *= t;
    RenderCommand fill_cmd;
    fill_cmd.type = RenderCommand::Type::Rect;
    fill_cmd.bounds = fill_bounds;
    fill_cmd.color = Colors::Accent;
    fill_cmd.cornerRadius = CornerRadius(3.0f);
    commands_.push_back(fill_cmd);

    // Label
    RenderCommand text_cmd;
    text_cmd.type = RenderCommand::Type::Text;
    text_cmd.bounds = {slider_bounds.right() + 12, bounds.y, 100, bounds.height};
    text_cmd.text = label;
    text_cmd.fontSize = 12.0f;
    text_cmd.textColor = Colors::LightGray;
    commands_.push_back(text_cmd);

    push_element_bounds(bounds);

    // Drag interaction
    if (renderer_.get_input().mouse_button_down(MouseButton::Left) && state.hovered) {
        float mx = renderer_.get_input().mouse_position().x;
        t = std::clamp((mx - slider_bounds.x) / slider_bounds.width, 0.0f, 1.0f);
        *value = min + t * (max - min);
    }

    // Reset style
    current_style_ = Style{};

    return false;
}

bool Builder::separator(float thickness) {
    auto bounds = calculate_element_bounds("", 0, thickness);

    RenderCommand cmd;
    cmd.type = RenderCommand::Type::Rect;
    cmd.bounds = bounds;
    cmd.color = Colors::DarkGray;
    commands_.push_back(cmd);

    push_element_bounds(bounds);

    // Reset style
    current_style_ = Style{};

    return false;
}

// ============================================================================
// Interaction state
// ============================================================================

bool Builder::hovered() const {
    if (bounds_stack_.empty()) return false;
    auto mouse = renderer_.get_input().mouse_position();
    return bounds_stack_.back().contains(mouse);
}

bool Builder::clicked() const {
    return hovered() && renderer_.get_input().mouse_button_pressed(MouseButton::Left);
}

bool Builder::active() const {
    return hovered() && renderer_.get_input().mouse_button_down(MouseButton::Left);
}

// ============================================================================
// Rendering
// ============================================================================

void Builder::render_all(Renderer& renderer) {
    for (const auto& cmd : commands_) {
        switch (cmd.type) {
            case RenderCommand::Type::Rect:
                renderer.draw_rect(cmd.bounds, cmd.color, cmd.cornerRadius);
                break;
            case RenderCommand::Type::Text:
                renderer.draw_text(cmd.text, cmd.bounds.x, cmd.bounds.y, cmd.fontSize, cmd.textColor);
                break;
            case RenderCommand::Type::Border:
                renderer.draw_border(cmd.bounds, cmd.color, cmd.borderWidth, cmd.cornerRadius);
                break;
            default:
                break;
        }
    }
}

} // namespace cui::ui
