#pragma once

#include <cui/window.h>
#include <cui/renderer.h>
#include <cui/input.h>
#include <cui/hot_reload.h>
#include <string>
#include <functional>
#include <memory>

namespace cui {

struct AppConfig {
    std::string title = "CUIFramework";
    int width = 1280;
    int height = 720;
    bool vsync = true;
    HotReloadConfig hot_reload;
};

class App {
public:
    App();
    ~App();

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    bool init(const AppConfig& config);
    void run();
    void shutdown();

    Window& window() { return *window_; }
    Renderer& renderer() { return *renderer_; }
    Input& input() { return input_; }

    using UpdateCallback = std::function<void(float delta_time)>;
    using RenderCallback = std::function<void()>;

    void set_update_callback(UpdateCallback cb) { update_cb_ = std::move(cb); }
    void set_render_callback(RenderCallback cb) { render_cb_ = std::move(cb); }

private:
    std::unique_ptr<Window> window_;
    std::unique_ptr<Renderer> renderer_;
    Input input_;
    HotReload hot_reload_;
    UpdateCallback update_cb_;
    RenderCallback render_cb_;
    bool running_ = false;
};

// Convenience: simple run loop
int run_app(const AppConfig& config,
            std::function<void(App&)> setup,
            std::function<void(App&, float)> update,
            std::function<void(App&)> render);

} // namespace cui
