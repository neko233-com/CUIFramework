#include <cui/guest/api.h>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>

// ============================================================================
// Hot-reloadable UI code
// Edit this file and save to see changes without restarting the app
// ============================================================================

static float anim_time = 0.0f;
static int click_count = 0;
static float slider_value = 0.5f;
static bool show_panel = true;

extern "C" {

CUI_API void cui_init(struct CUIContext* ctx) {
    anim_time = 0.0f;
    click_count = 0;
    slider_value = 0.5f;
    show_panel = true;
    std::printf("[UI] Hot-reloadable UI initialized! Screen: %.0fx%.0f\n",
                ctx->screen_width, ctx->screen_height);
}

CUI_API int cui_update(struct CUIContext* ctx) {
    anim_time += ctx->delta_time;

    // Handle input
    if (ctx->mouse_buttons & 1) {
        if (ctx->mouse_x > 100 && ctx->mouse_x < 300 &&
            ctx->mouse_y > 200 && ctx->mouse_y < 240) {
            click_count++;
        }
    }

    // Update slider
    if (ctx->mouse_buttons & 1) {
        if (ctx->mouse_x > 100 && ctx->mouse_x < 250 &&
            ctx->mouse_y > 260 && ctx->mouse_y < 280) {
            slider_value = (ctx->mouse_x - 100.0f) / 150.0f;
            slider_value = std::clamp(slider_value, 0.0f, 1.0f);
        }
    }

    // Toggle panel with space
    if (ctx->keys_just_pressed[' ']) {
        show_panel = !show_panel;
    }

    return 0;
}

CUI_API void cui_shutdown(struct CUIContext* ctx) {
    std::printf("[UI] Hot-reloadable UI shutdown after %.2f seconds\n", ctx->total_time);
}

} // extern "C"
