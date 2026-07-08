// examples/basic/basic.cpp
#include <cui/app.h>
#include <cui/ui/builder.h>
#include <cstdio>

int main() {
    cui::AppConfig config;
    config.title = "CUIFramework - Basic Example";
    config.width = 1280;
    config.height = 720;

    return cui::run_app(config,
        [](cui::App& app) {
            std::printf("CUIFramework Basic Example started!\n");
        },
        [](cui::App& app, float dt) {},
        [](cui::App& app) {
            auto& renderer = app.renderer();
            auto& input = app.input();
            auto& window = app.window();

            renderer.draw_rect({0, 0, (float)window.width(), (float)window.height()}, cui::Colors::Background);
            renderer.draw_text("CUIFramework - Basic Example", 50, 50, 28, cui::Colors::White);

            cui::ui::Builder builder(renderer, input, (float)window.width(), (float)window.height());
            builder.begin("root", cui::FlexDirection::Column, 16, cui::Padding(32));
            {
                builder.text("Welcome to CUIFramework!", 20);
                if (builder.button("Click Me")) std::printf("Button clicked!\n");
                static float slider_val = 0.5f;
                builder.slider("Opacity", &slider_val, 0.0f, 1.0f);
                static bool checked = false;
                builder.checkbox("Enable Feature", &checked);
                builder.separator();
                builder.text("This is a simple UI example.", 14);
            }
            builder.end();
            builder.render_all(renderer);
        }
    );
}
