#include <cui/window.h>
#include <SDL.h>
#include <stdexcept>
#include <string>

namespace cui {

class WindowImpl {
public:
    bool init(const std::string& title, int width, int height) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
            throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#ifdef __APPLE__
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

        window_ = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN
        );

        if (!window_) {
            throw std::runtime_error(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
        }

        gl_context_ = SDL_GL_CreateContext(window_);
        if (!gl_context_) {
            throw std::runtime_error(std::string("SDL_GL_CreateContext failed: ") + SDL_GetError());
        }

        SDL_GL_SetSwapInterval(1); // vsync

        width_ = width;
        height_ = height;
        return true;
    }

    void shutdown() {
        if (gl_context_) {
            SDL_GL_DeleteContext(gl_context_);
            gl_context_ = nullptr;
        }
        if (window_) {
            SDL_DestroyWindow(window_);
            window_ = nullptr;
        }
        SDL_Quit();
    }

    bool is_open() const { return window_ != nullptr; }

    void poll_events() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    SDL_DestroyWindow(window_);
                    window_ = nullptr;
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        width_ = event.window.data1;
                        height_ = event.window.data2;
                    }
                    break;
                case SDL_KEYDOWN:
                    keys_[event.key.keysym.scancode] = true;
                    break;
                case SDL_KEYUP:
                    keys_[event.key.keysym.scancode] = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouse_buttons_[event.button.button - 1] = true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouse_buttons_[event.button.button - 1] = false;
                    break;
                case SDL_MOUSEMOTION:
                    mouse_x_ = static_cast<float>(event.motion.x);
                    mouse_y_ = static_cast<float>(event.motion.y);
                    break;
                case SDL_MOUSEWHEEL:
                    mouse_wheel_ += event.wheel.y;
                    break;
            }
        }
    }

    void swap_buffers() {
        SDL_GL_SwapWindow(window_);
    }

    int width() const { return width_; }
    int height() const { return height_; }
    Vec2 mouse_position() const { return {mouse_x_, mouse_y_}; }
    bool mouse_button_down(int button) const {
        if (button < 0 || button > 2) return false;
        return mouse_buttons_[button];
    }
    bool key_down(int scancode) const {
        if (scancode < 0 || scancode >= 512) return false;
        return keys_[scancode];
    }

    void* native_handle() const { return window_; }

private:
    SDL_Window* window_ = nullptr;
    SDL_GLContext gl_context_ = nullptr;
    int width_ = 0;
    int height_ = 0;
    float mouse_x_ = 0.0f;
    float mouse_y_ = 0.0f;
    float mouse_wheel_ = 0.0f;
    bool mouse_buttons_[3] = {};
    bool keys_[512] = {};
};

Window::Window(const std::string& title, int width, int height)
    : impl_(std::make_unique<WindowImpl>())
{
    impl_->init(title, width, height);
}

Window::~Window() {
    if (impl_) {
        impl_->shutdown();
    }
}

bool Window::is_open() const { return impl_->is_open(); }
void Window::poll_events() { impl_->poll_events(); }
void Window::swap_buffers() { impl_->swap_buffers(); }
int Window::width() const { return impl_->width(); }
int Window::height() const { return impl_->height(); }
Vec2 Window::mouse_position() const { return impl_->mouse_position(); }
bool Window::mouse_button_down(int button) const { return impl_->mouse_button_down(button); }
bool Window::key_down(int key) const { return impl_->key_down(key); }
void* Window::native_handle() const { return impl_->native_handle(); }

} // namespace cui
