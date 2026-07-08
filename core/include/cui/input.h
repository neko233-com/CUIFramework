#pragma once

#include <cui/layout_types.h>
#include <cstdint>

namespace cui {

enum class KeyCode : uint32_t {
    Unknown = 0,
    Space = 32, Escape = 27, Return = 13, Tab = 9,
    Backspace = 8, Delete = 127,
    Left = 80, Right = 78, Up = 77, Down = 81,
    A = 97, B = 98, C = 99, D = 100, E = 101, F = 102,
    G = 103, H = 104, I = 105, J = 106, K = 107, L = 108,
    M = 109, N = 110, O = 111, P = 112, Q = 113, R = 114,
    S = 115, T = 116, U = 117, V = 118, W = 119, X = 120,
    Y = 121, Z = 122,
    Num0 = 48, Num1 = 49, Num2 = 50, Num3 = 51, Num4 = 52,
    Num5 = 53, Num6 = 54, Num7 = 55, Num8 = 56, Num9 = 57
};

enum class MouseButton : uint8_t {
    Left = 0,
    Right = 1,
    Middle = 2
};

class Input {
public:
    void reset_frame();
    void set_mouse_position(Vec2 pos);
    void set_mouse_button(MouseButton btn, bool pressed);
    void set_key(KeyCode key, bool pressed);

    Vec2 mouse_position() const { return mouse_pos_; }
    Vec2 mouse_delta() const { return mouse_delta_; }
    float mouse_wheel() const { return mouse_wheel_; }
    bool mouse_button_down(MouseButton btn) const;
    bool mouse_button_pressed(MouseButton btn) const;
    bool mouse_button_released(MouseButton btn) const;
    bool key_down(KeyCode key) const;
    bool key_pressed(KeyCode key) const;
    bool key_released(KeyCode key) const;

private:
    Vec2 mouse_pos_{0, 0};
    Vec2 mouse_delta_{0, 0};
    Vec2 prev_mouse_pos_{0, 0};
    float mouse_wheel_ = 0.0f;
    bool mouse_buttons_[3] = {};
    bool prev_mouse_buttons_[3] = {};
    bool keys_[256] = {};
    bool prev_keys_[256] = {};
};

} // namespace cui
