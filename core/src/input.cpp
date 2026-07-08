#include <cui/input.h>

namespace cui {

void Input::reset_frame() {
    mouse_delta_ = mouse_pos_ - prev_mouse_pos_;
    prev_mouse_pos_ = mouse_pos_;
    mouse_wheel_ = 0.0f;

    for (int i = 0; i < 3; i++) prev_mouse_buttons_[i] = mouse_buttons_[i];
    for (int i = 0; i < 256; i++) prev_keys_[i] = keys_[i];
}

void Input::set_mouse_position(Vec2 pos) { mouse_pos_ = pos; }
void Input::set_mouse_button(MouseButton btn, bool pressed) {
    mouse_buttons_[static_cast<uint8_t>(btn)] = pressed;
}
void Input::set_key(KeyCode key, bool pressed) {
    auto idx = static_cast<uint32_t>(key);
    if (idx < 256) keys_[idx] = pressed;
}

bool Input::mouse_button_down(MouseButton btn) const {
    return mouse_buttons_[static_cast<uint8_t>(btn)];
}
bool Input::mouse_button_pressed(MouseButton btn) const {
    auto i = static_cast<uint8_t>(btn);
    return mouse_buttons_[i] && !prev_mouse_buttons_[i];
}
bool Input::mouse_button_released(MouseButton btn) const {
    auto i = static_cast<uint8_t>(btn);
    return !mouse_buttons_[i] && prev_mouse_buttons_[i];
}
bool Input::key_down(KeyCode key) const {
    auto idx = static_cast<uint32_t>(key);
    return idx < 256 && keys_[idx];
}
bool Input::key_pressed(KeyCode key) const {
    auto idx = static_cast<uint32_t>(key);
    return idx < 256 && keys_[idx] && !prev_keys_[idx];
}
bool Input::key_released(KeyCode key) const {
    auto idx = static_cast<uint32_t>(key);
    return idx < 256 && !keys_[idx] && prev_keys_[idx];
}

} // namespace cui
