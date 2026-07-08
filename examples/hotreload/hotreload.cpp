// examples/hotreload/hotreload.cpp — Hot reload demo
// Modifies guest/src/ui_main.cpp and saves to see live updates
#include <cui/app.h>
#include <cui/renderer.h>
#include <cui/hot_reload.h>
#include <cui/ui/builder.h>
#include <cstdio>
#include <string>

int main(int argc, char* argv[]) {
    cui::AppConfig config;
    config.title = "CUIFramework - Hot Reload Demo";
    config.width = 1280;
    config.height = 720;

    // Guest DLL path — look in same directory as executable
    std::string guest_dll = "cui_guest";
#ifdef _WIN32
    guest_dll += ".dll";
#elif __APPLE__
    guest_dll = "libcui_guest.dylib";
#else
    guest_dll = "libcui_guest.so";
#endif

    config.hot_reload.guest_dll_path = guest_dll;
    config.hot_reload.enable_crash_protection = true;

    cui::App app;
    if (!app.init(config)) {
        std::fprintf(stderr, "Failed to initialize app\n");
        return -1;
    }

    int frame = 0;
    float fps_timer = 0;
    int fps_count = 0;

    // UI state
    static float slider_val = 0.5f;
    static bool show_debug = true;
    static int click_count = 0;

    app.set_update_callback([&](float dt) {
        frame++;
        fps_timer += dt;
        fps_count++;

        if (fps_timer >= 1.0f) {
            std::printf("FPS: %d | Frame: %d\n", fps_count, frame);
            fps_timer = 0;
            fps_count = 0;
        }
    });

    app.set_render_callback([&]() {
        auto& renderer = app.renderer();
        auto& window = app.window();

        // Background
        renderer.draw_rect({0, 0, (float)window.width(), (float)window.height()},
                         cui::Colors::Background);

        // Title
        renderer.draw_text("CUIFramework - Hot Reload Demo",
                        50, 30, 28, cui::Colors::White);

        // Instructions
        renderer.draw_text("Edit guest/src/ui_main.cpp and save to see live updates!",
                        50, 70, 16, cui::Colors::LightGray);

        renderer.draw_text("The UI will update without restarting the application.",
                        50, 95, 16, cui::Colors::LightGray);

        // Hot reload status
        renderer.draw_text("Hot Reload: Active",
                        50, 140, 16, cui::Colors::Green);

        // UI Panel
        cui::ui::Builder builder(renderer,
                                (float)window.width(),
                                (float)window.height());

        builder.begin("panel", cui::FlexDirection::Column, 12, cui::Padding(50, 50, 50, 50));
        {
            builder.text("Interactive Panel", 20);

            builder.separator();

            if (builder.button("Click Count: " + std::to_string(click_count))) {
                click_count++;
                std::printf("Button clicked! Count: %d\n", click_count);
            }

            builder.slider("Slider Value", &slider_val, 0.0f, 1.0f);

            builder.checkbox("Show Debug Info", &show_debug);

            if (show_debug) {
                builder.separator();
                builder.text("Debug Info:", 14);
                builder.text("  Frame: " + std::to_string(frame), 12);
                builder.text("  FPS: " + std::to_string(fps_count), 12);
                builder.text("  Slider: " + std::to_string(slider_val), 12);
            }
        }
        builder.end();

        builder.render_all(renderer);
    });

    app.run();
    return 0;
}
