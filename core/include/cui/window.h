#pragma once

#include <cui/layout_types.h>
#include <string>
#include <memory>

namespace cui {

class WindowImpl;

class Window {
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool is_open() const;
    void poll_events();
    void swap_buffers();

    int width() const;
    int height() const;
    Vec2 mouse_position() const;
    bool mouse_button_down(int button) const;
    bool key_down(int key) const;

    void* native_handle() const;

private:
    std::unique_ptr<WindowImpl> impl_;
};

} // namespace cui
