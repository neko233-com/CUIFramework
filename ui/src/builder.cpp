#include <cui/ui/builder.h>
#include <algorithm>

namespace cui::ui {

Builder::Builder(Renderer& renderer, Input& input, float screen_width, float screen_height)
    : renderer_(renderer)
    , input_(input)
    , screen_width_(screen_width)
    , screen_height_(screen_height)
{
    bounds_stack_.push_back({0, 0, screen_width, screen_height});
}

Builder::~Builder() = default;

void Builder::begin(const std::string& id, FlexDirection direction, float gap, Padding padding) {
    bounds_stack_.push_back(bounds_stack_.back());
}

void Builder::end() {
    if (!bounds_stack_.empty()) {
        bounds_stack_.pop_back();
    }
}

Builder& Builder::background(Color color) { current_style_.background = color; return *this; }
Builder& Builder::text_color(Color color) { current_style_.text_color = color; return *this; }
Builder& Builder::corner_radius(float radius) { current_style_.corner_radius = CornerRadius(radius); return *this; }
Builder& Builder::padding(Padding padding) { current_style_.padding = padding; return *this; }
Builder& Builder::gap(float gap) { current_style_.gap = gap; return *this; }
Builder& Builder::width(float w) { current_style_.width = w; return *this; }
Builder& Builder::height(float h) { current_style_.height = h; return *this; }
Builder& Builder::size(float w, float h) { current_style_.width = w; current_style_.height = h; return *this; }

Rect Builder::calculate_element_bounds(float width, float height) {
    if (bounds_stack_.empty()) return {0, 0, width, height};
    auto& parent = bounds_stack_.back();
    return {parent.x + current_style_.padding.left,
            parent.y + current_style_.padding.top,
            width > 0 ? width : parent.width,
            height > 0 ? height : 30.0f};
}

void Builder::push_element_bounds(const Rect& bounds) {
    bounds_stack_.push_back(bounds);
}

bool Builder::text(const std::string& content, float font_size) {
    auto bounds = calculate_element_bounds(content.length() * font_size * 0.6f, font_size);
    commands_.push_back({RenderCommand::Type::Text, bounds, {}, {}, content, font_size, current_style_.text_color});
    current_style_ = Style{};
    return false;
}

bool Builder::button(const std::string& label) {
    auto bounds = calculate_element_bounds(120, 32);
    bool hovered = bounds.contains({input_.mouse_position().x, input_.mouse_position().y});
    bool pressed = hovered && input_.mouse_button_pressed(MouseButton::Left);

    Color bg = pressed ? Colors::Accent : (hovered ? Color{80, 130, 220, 255} : Colors::DarkGray);
    commands_.push_back({RenderCommand::Type::Rect, bounds, bg, CornerRadius(4.0f)});
    commands_.push_back({RenderCommand::Type::Text, {bounds.x + 8, bounds.y + 8, bounds.width - 16, bounds.height - 16}, {}, {}, label, 14.0f, Colors::White});

    current_style_ = Style{};
    return pressed;
}

bool Builder::checkbox(const std::string& label, bool* value) {
    auto bounds = calculate_element_bounds(200, 24);
    bool hovered = bounds.contains({input_.mouse_position().x, input_.mouse_position().y});

    Rect check_bounds = {bounds.x, bounds.y + 4, 16, 16};
    commands_.push_back({RenderCommand::Type::Rect, check_bounds, *value ? Colors::Accent : Colors::DarkGray, CornerRadius(3.0f)});
    commands_.push_back({RenderCommand::Type::Text, {check_bounds.right() + 8, bounds.y, 150, bounds.height}, {}, {}, label, 14.0f, Colors::White});

    if (hovered && input_.mouse_button_pressed(MouseButton::Left)) {
        *value = !(*value);
    }

    current_style_ = Style{};
    return *value;
}

bool Builder::slider(const std::string& label, float* value, float min, float max) {
    auto bounds = calculate_element_bounds(200, 32);
    bool hovered = bounds.contains({input_.mouse_position().x, input_.mouse_position().y});

    Rect slider_bounds = {bounds.x, bounds.y + 13, 150, 6};
    commands_.push_back({RenderCommand::Type::Rect, slider_bounds, Colors::DarkGray, CornerRadius(3.0f)});

    float t = std::clamp((*value - min) / (max - min), 0.0f, 1.0f);
    Rect fill_bounds = slider_bounds;
    fill_bounds.width *= t;
    commands_.push_back({RenderCommand::Type::Rect, fill_bounds, Colors::Accent, CornerRadius(3.0f)});
    commands_.push_back({RenderCommand::Type::Text, {slider_bounds.right() + 12, bounds.y, 100, bounds.height}, {}, {}, label, 12.0f, Colors::LightGray});

    if (input_.mouse_button_down(MouseButton::Left) && hovered) {
        float mx = input_.mouse_position().x;
        t = std::clamp((mx - slider_bounds.x) / slider_bounds.width, 0.0f, 1.0f);
        *value = min + t * (max - min);
    }

    current_style_ = Style{};
    return false;
}

bool Builder::separator(float thickness) {
    auto bounds = calculate_element_bounds(0, thickness);
    commands_.push_back({RenderCommand::Type::Rect, bounds, Colors::DarkGray});
    current_style_ = Style{};
    return false;
}

bool Builder::hovered() const {
    if (bounds_stack_.empty()) return false;
    return bounds_stack_.back().contains({input_.mouse_position().x, input_.mouse_position().y});
}

bool Builder::clicked() const {
    return hovered() && input_.mouse_button_pressed(MouseButton::Left);
}

bool Builder::active() const {
    return hovered() && input_.mouse_button_down(MouseButton::Left);
}

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
