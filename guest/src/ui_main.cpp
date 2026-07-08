#include <cui/guest/api.h>
#include <cstdio>
#include <cstring>
#include <cmath>

// ============================================================================
// Hot-reloadable UI code
// Edit this file and save to see changes without restarting the app
// ============================================================================

// UI State (automatically transferred across reloads)
static float anim_time = 0.0f;
static float color_r = 0.2f;
static float color_g = 0.4f;
static float color_b = 0.8f;
static int selected_tab = 0;
static char text_buffer[256] = "Edit me!";

// ============================================================================
// UI Components (hot-reloadable)
// ============================================================================

static void draw_button(float x, float y, float w, float h, const char* label,
                       float r, float g, float b, bool hovered, bool pressed) {
    // This function draws using the host's renderer
    // The actual rendering happens through the host's draw calls
    // For now, we just output debug info
    std::printf("Button: %s at (%.0f, %.0f) size (%.0f, %.0f) [%s%s]\n",
                label, x, y, w, h,
                hovered ? "hovered" : "",
                pressed ? "pressed" : "");
}

static void draw_slider(float x, float y, float w, float h, float value,
                       float r, float g, float b) {
    std::printf("Slider: value=%.2f at (%.0f, %.0f)\n", value, x, y);
}

// ============================================================================
// Guest API implementation
// ============================================================================

extern "C" {

CUI_API void cui_init(struct CUIContext* ctx) {
    anim_time = 0.0f;
    color_r = 0.2f;
    color_g = 0.4f;
    color_b = 0.8f;
    selected_tab = 0;
    std::snprintf(text_buffer, sizeof(text_buffer), "Hello from hot-reloadable UI!");

    std::printf("[UI] Hot-reloadable UI initialized!\n");
    std::printf("[UI] Edit this file and save to see changes!\n");
}

CUI_API int cui_update(struct CUIContext* ctx) {
    anim_time += ctx->delta_time;

    // Animate colors
    color_r = 0.5f + 0.3f * std::sin(anim_time * 0.5f);
    color_g = 0.5f + 0.3f * std::sin(anim_time * 0.7f + 1.0f);
    color_b = 0.5f + 0.3f * std::sin(anim_time * 0.3f + 2.0f);

    // Handle tab selection
    if (ctx->keys_just_pressed['1']) selected_tab = 0;
    if (ctx->keys_just_pressed['2']) selected_tab = 1;
    if (ctx->keys_just_pressed['3']) selected_tab = 2;

    // Draw UI using host renderer
    // Note: In a real implementation, you'd call the host's draw functions
    // through function pointers or a vtable. For now, we use printf.

    float mx = ctx->mouse_x;
    float my = ctx->mouse_y;
    bool left_click = (ctx->mouse_buttons & 1) != 0;

    // Tab bar
    const char* tabs[] = {"Home", "Settings", "About"};
    for (int i = 0; i < 3; i++) {
        float tab_x = 50 + i * 100.0f;
        float tab_y = 50;
        bool tab_hovered = (mx >= tab_x && mx <= tab_x + 90 &&
                          my >= tab_y && my <= tab_y + 30);
        bool tab_selected = (selected_tab == i);

        if (tab_hovered && left_click) {
            selected_tab = i;
        }

        // Host would render: colored rect + text
        draw_button(tab_x, tab_y, 90, 30, tabs[i],
                   tab_selected ? 0.3f : 0.2f,
                   tab_selected ? 0.5f : 0.2f,
                   tab_selected ? 0.9f : 0.2f,
                   tab_hovered, left_click && tab_hovered);
    }

    // Content area
    float content_y = 100;
    switch (selected_tab) {
        case 0: // Home
            draw_button(50, content_y, 200, 40, "Click Me!",
                       color_r, color_g, color_b,
                       mx > 50 && mx < 250 && my > content_y && my < content_y + 40,
                       left_click);
            break;

        case 1: // Settings
            draw_slider(50, content_y, 200, 20, color_r, 0.8f, 0.2f, 0.2f);
            draw_slider(50, content_y + 40, 200, 20, color_g, 0.2f, 0.8f, 0.2f);
            draw_slider(50, content_y + 80, 200, 20, color_b, 0.2f, 0.2f, 0.8f);
            break;

        case 2: // About
            std::printf("CUIFramework v0.1.0\n");
            std::printf("A C++23 cross-platform UI framework with hot reload\n");
            break;
    }

    return 0;
}

CUI_API void cui_shutdown(struct CUIContext* ctx) {
    std::printf("[UI] Hot-reloadable UI shutdown\n");
}

} // extern "C"
