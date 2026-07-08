// examples/hotreload/hotreload.cpp — Hot reload demo
// Modifies guest/src/ui_main.cpp and saves to see live updates
#include <cui/app.h>
#include <cui/renderer.h>
#include <cui/hot_reload.h>
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
                        50, 50, 28, cui::Colors::White);

        // Instructions
        renderer.draw_text("Edit guest/src/ui_main.cpp and save to see live updates!",
                        50, 100, 16, cui::Colors::LightGray);

        renderer.draw_text("The UI will update without restarting the application.",
                        50, 130, 16, cui::Colors::LightGray);

        // Hot reload status
        renderer.draw_text("Hot Reload: Active",
                        50, 180, 16, cui::Colors::Green);
    });

    app.run();
    return 0;
}
