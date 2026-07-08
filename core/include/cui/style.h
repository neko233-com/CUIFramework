#pragma once

#include <cstdint>
#include <cstddef>

namespace cui {

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;
};

struct Vec4 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;
};

struct Rect {
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;

    float right() const { return x + width; }
    float bottom() const { return y + height; }
    bool contains(Vec2 p) const {
        return p.x >= x && p.x <= right() && p.y >= y && p.y <= bottom();
    }
};

struct Color {
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    uint8_t a = 255;

    float rf() const { return r / 255.0f; }
    float gf() const { return g / 255.0f; }
    float bf() const { return b / 255.0f; }
    float af() const { return a / 255.0f; }

    static Color from_rgb(uint8_t r, uint8_t g, uint8_t b) { return {r, g, b, 255}; }
    static Color from_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { return {r, g, b, a}; }
    static Color from_hex(uint32_t hex) {
        return {
            static_cast<uint8_t>((hex >> 16) & 0xFF),
            static_cast<uint8_t>((hex >> 8) & 0xFF),
            static_cast<uint8_t>(hex & 0xFF),
            255
        };
    }
};

// Predefined colors
namespace Colors {
    inline constexpr Color White{255, 255, 255, 255};
    inline constexpr Color Black{0, 0, 0, 255};
    inline constexpr Color Red{255, 0, 0, 255};
    inline constexpr Color Green{0, 255, 0, 255};
    inline constexpr Color Blue{0, 0, 255, 255};
    inline constexpr Color Transparent{0, 0, 0, 0};
    inline constexpr Color Gray{128, 128, 128, 255};
    inline constexpr Color DarkGray{64, 64, 64, 255};
    inline constexpr Color LightGray{192, 192, 192, 255};
    inline constexpr Color Background{30, 30, 30, 255};
    inline constexpr Color Panel{40, 40, 40, 255};
    inline constexpr Color Accent{100, 149, 237, 255};
}

struct Padding {
    float top = 0.0f;
    float right = 0.0f;
    float bottom = 0.0f;
    float left = 0.0f;

    Padding() = default;
    explicit Padding(float all) : top(all), right(all), bottom(all), left(all) {}
    Padding(float vertical, float horizontal)
        : top(vertical), right(horizontal), bottom(vertical), left(horizontal) {}
    Padding(float t, float r, float b, float l) : top(t), right(r), bottom(b), left(l) {}
};

struct CornerRadius {
    float tl = 0.0f;
    float tr = 0.0f;
    float br = 0.0f;
    float bl = 0.0f;

    CornerRadius() = default;
    explicit CornerRadius(float all) : tl(all), tr(all), br(all), bl(all) {}
};

struct BorderWidth {
    float top = 0.0f;
    float right = 0.0f;
    float bottom = 0.0f;
    float left = 0.0f;

    BorderWidth() = default;
    explicit BorderWidth(float all) : top(all), right(all), bottom(all), left(all) {}
};

} // namespace cui
