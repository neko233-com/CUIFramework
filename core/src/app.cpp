#include <cui/app.h>
#include <chrono>

namespace cui {

App::App() = default;
App::~App() { shutdown(); }

bool App::init(const AppConfig& config) {
    window_ = std::make_unique<Window>(config.title, config.width, config.height);
    renderer_ = std::make_unique<Renderer>();
    renderer_->init(window_->native_handle());

    if (!config.hot_reload.guest_dll_path.empty()) {
        hot_reload_.init(config.hot_reload);
    }

    return true;
}

void App::run() {
    running_ = true;
    auto last_time = std::chrono::high_resolution_clock::now();

    while (window_->is_open() && running_) {
        auto now = std::chrono::high_resolution_clock::now();
        float delta_time = std::chrono::duration<float>(now - last_time).count();
        last_time = now;

        // Poll events
        window_->poll_events();
        input_.reset_frame();

        // Update hot reload
        hot_reload_.update(delta_time);

        // User update callback
        if (update_cb_) update_cb_(delta_time);

        // Render
        renderer_->begin_frame();
        if (render_cb_) render_cb_();

        // Hot reload guest render
        if (hot_reload_.is_loaded()) {
            // Guest rendering happens here via hot reload callbacks
        }

        renderer_->end_frame();
        window_->swap_buffers();
    }
}

void App::shutdown() {
    hot_reload_.shutdown();
    if (renderer_) renderer_->shutdown();
    window_.reset();
}

int run_app(const AppConfig& config,
            std::function<void(App&)> setup,
            std::function<void(App&, float)> update,
            std::function<void(App&)> render)
{
    App app;
    if (!app.init(config)) return -1;

    if (setup) setup(app);

    app.set_update_callback([&](float dt) {
        if (update) update(app, dt);
    });
    app.set_render_callback([&]() {
        if (render) render(app);
    });

    app.run();
    return 0;
}

} // namespace cui
