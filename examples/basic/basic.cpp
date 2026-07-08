// examples/basic/basic.cpp — Minimal CUIFramework example
#include <cui/app.h>
#include <cui/renderer.h>
#include <cui/ui/builder.h>
#include <cstdio>

int main() {
    cui::AppConfig config;
    config.title = "CUIFramework - Basic Example";
    config.width = 1280;
    config.height = 720;

    return cui::run_app(config,
        // Setup
        [](cui::App& app) {
            std::printf("CUIFramework Basic Example started!\n");
        },
        // Update
        [](cui::App& app, float dt) {
            // Handle input here
        },
        // Render
        [](cui::App& app) {
            auto& renderer = app.renderer();
            auto& window = app.window();

            // Background
            renderer.draw_rect({0, 0, (float)window.width(), (float)window.height()},
                             cui::Colors::Background);

            // Title
            renderer.draw_text("CUIFramework - Basic Example",
                            50, 50, 28, cui::Colors::White);

            // Declarative UI
            cui::ui::Builder builder(renderer,
                                    (float)window.width(),
                                    (float)window.height());

            builder.begin_layout(cui::FlexDirection::Column, 16, cui::Padding(32));
            {
                builder.text("Welcome to CUIFramework!", 20, cui::Colors::Accent);

                builder.button("Click Me", cui::Colors::Accent);

                float slider_val = 0.5f;
                builder.slider("Opacity", &slider_val, 0.0f, 1.0f);

                bool checked = false;
                builder.checkbox("Enable Feature", &checked);
            }
            builder.end_layout();
        }
    );
}
