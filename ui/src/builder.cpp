#include <cui/ui/builder.h>
#include <cui/renderer.h>
#include <algorithm>

namespace cui::ui {

Builder::Builder(Renderer& renderer, float screen_width, float screen_height)
    : renderer_(renderer)
    , screen_width_(screen_width)
    , screen_height_(screen_height)
{
    bounds_stack_.push_back({0, 0, screen_width, screen_height});
}

Builder::~Builder() = default;

void Builder::begin_layout(FlexDirection direction, float gap, Padding padding) {
    StackFrame frame;
    frame.config.direction = direction;
    frame.config.gap = gap;
    frame.config.padding = padding;
    frame.bounds = bounds_stack_.back();
    stack_.push_back(std::move(frame));
}

void Builder::end_layout() {
    if (!stack_.empty()) {
        stack_.pop_back();
    }
    if (!bounds_stack_.empty()) {
        bounds_stack_.pop_back();
    }
}

void Builder::set_padding(Padding padding) {
    if (!stack_.empty()) {
        stack_.back().config.padding = padding;
    }
}

void Builder::set_gap(float gap) {
    if (!stack_.empty()) {
        stack_.back().config.gap = gap;
    }
}

void Builder::set_size(float width, float height) {
    if (!bounds_stack_.empty()) {
        auto& b = bounds_stack_.back();
        if (width > 0) b.width = width;
        if (height > 0) b.height = height;
    }
}

void Builder::set_background(Color color) {
    if (!bounds_stack_.empty()) {
        RenderCommand cmd;
        cmd.type = RenderCommand::Type::Rect;
        cmd.bounds = bounds_stack_.back();
        cmd.color = color;
        commands_.push_back(cmd);
    }
}

void Builder::set_corner_radius(CornerRadius radius) {
    // Applied to next draw command
}

bool Builder::box(const std::string& id) {
    auto parent = bounds_stack_.back();
    auto& frame = stack_.back();

    Rect box_bounds;
    float offset = 0;

    if (frame.config.direction == FlexDirection::Column) {
        box_bounds.x = parent.x + frame.config.padding.left;
        box_bounds.y = parent.y + frame.config.padding.top + offset;
        box_bounds.width = parent.width - frame.config.padding.left - frame.config.padding.right;
        box_bounds.height = 50; // Default, or grow
    } else {
        box_bounds.x = parent.x + frame.config.padding.left + offset;
        box_bounds.y = parent.y + frame.config.padding.top;
        box_bounds.width = 50;
        box_bounds.height = parent.height - frame.config.padding.top - frame.config.padding.bottom;
    }

    bounds_stack_.push_back(box_bounds);
    return true;
}

bool Builder::text(const std::string& content, float font_size, Color color) {
    auto parent = bounds_stack_.back();
    auto& frame = stack_.back();

    Rect text_bounds;
    float text_width = content.length() * font_size * 0.6f;
    float text_height = font_size;

    if (frame.config.direction == FlexDirection::Column) {
        text_bounds.x = parent.x + frame.config.padding.left;
        text_bounds.y = parent.y + frame.config.padding.top;
        text_bounds.width = text_width;
        text_bounds.height = text_height;
    } else {
        text_bounds.x = parent.x + frame.config.padding.left;
        text_bounds.y = parent.y + frame.config.padding.top;
        text_bounds.width = text_width;
        text_bounds.height = text_height;
    }

    RenderCommand cmd;
    cmd.type = RenderCommand::Type::Text;
    cmd.bounds = text_bounds;
    cmd.text = content;
    cmd.fontSize = font_size;
    cmd.textColor = color;
    commands_.push_back(cmd);

    return true;
}

bool Builder::button(const std::string& label, Color background, Color text_color) {
    auto parent = bounds_stack_.back();
    auto& frame = stack_.back();

    float btn_width = 120.0f;
    float btn_height = 32.0f;

    Rect btn_bounds;
    if (frame.config.direction == FlexDirection::Column) {
        btn_bounds.x = parent.x + frame.config.padding.left;
        btn_bounds.y = parent.y + frame.config.padding.top;
        btn_bounds.width = btn_width;
        btn_bounds.height = btn_height;
    } else {
        btn_bounds.x = parent.x + frame.config.padding.left;
        btn_bounds.y = parent.y + frame.config.padding.top;
        btn_bounds.width = btn_width;
        btn_bounds.height = btn_height;
    }

    RenderCommand cmd;
    cmd.type = RenderCommand::Type::Rect;
    cmd.bounds = btn_bounds;
    cmd.color = background;
    cmd.cornerRadius = CornerRadius(4.0f);
    commands_.push_back(cmd);

    // Label text centered in button
    RenderCommand text_cmd;
    text_cmd.type = RenderCommand::Type::Text;
    text_cmd.bounds = {btn_bounds.x + 8, btn_bounds.y + 6, btn_bounds.width - 16, btn_bounds.height - 12};
    text_cmd.text = label;
    text_cmd.fontSize = 14.0f;
    text_cmd.textColor = text_color;
    commands_.push_back(text_cmd);

    return hovered() && renderer_.get_input().mouse_button_pressed(MouseButton::Left);
}

bool Builder::checkbox(const std::string& label, bool* value) {
    auto parent = bounds_stack_.back();
    auto& frame = stack_.back();

    float box_size = 16.0f;
    Rect check_bounds;
    check_bounds.x = parent.x + frame.config.padding.left;
    check_bounds.y = parent.y + frame.config.padding.top;
    check_bounds.width = box_size;
    check_bounds.height = box_size;

    // Box background
    RenderCommand cmd;
    cmd.type = RenderCommand::Type::Rect;
    cmd.bounds = check_bounds;
    cmd.color = *value ? Colors::Accent : Colors::DarkGray;
    cmd.cornerRadius = CornerRadius(3.0f);
    commands_.push_back(cmd);

    // Label
    RenderCommand text_cmd;
    text_cmd.type = RenderCommand::Type::Text;
    text_cmd.bounds = {check_bounds.right() + 8, check_bounds.y, 200, box_size};
    text_cmd.text = label;
    text_cmd.fontSize = 14.0f;
    text_cmd.textColor = Colors::White;
    commands_.push_back(text_cmd);

    // Toggle on click
    if (hovered() && renderer_.get_input().mouse_button_pressed(MouseButton::Left)) {
        *value = !(*value);
        return true;
    }
    return false;
}

bool Builder::slider(const std::string& label, float* value, float min, float max) {
    auto parent = bounds_stack_.back();
    auto& frame = stack_.back();

    float slider_width = 150.0f;
    float slider_height = 6.0f;

    Rect slider_bounds;
    slider_bounds.x = parent.x + frame.config.padding.left;
    slider_bounds.y = parent.y + frame.config.padding.top;
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
    text_cmd.bounds = {slider_bounds.right() + 8, slider_bounds.y - 5, 200, 20};
    text_cmd.text = label;
    text_cmd.fontSize = 12.0f;
    text_cmd.textColor = Colors::LightGray;
    commands_.push_back(text_cmd);

    // Drag interaction
    if (renderer_.get_input().mouse_button_down(MouseButton::Left) && hovered()) {
        float mx = renderer_.get_input().mouse_position().x;
        t = std::clamp((mx - slider_bounds.x) / slider_bounds.width, 0.0f, 1.0f);
        *value = min + t * (max - min);
        return true;
    }
    return false;
}

bool Builder::hovered() const {
    if (bounds_stack_.empty()) return false;
    auto mouse = renderer_.get_input().mouse_position();
    return bounds_stack_.back().contains(mouse);
}

bool Builder::clicked() const {
    return hovered() && renderer_.get_input().mouse_button_pressed(MouseButton::Left);
}

} // namespace cui::ui
