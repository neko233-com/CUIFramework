#include <cui/guest/api.h>
#include <cstdio>
#include <cstring>

// ============================================================================
// Guest state (automatically transferred across reloads via cr.h)
// ============================================================================

static int frame_count = 0;
static int click_count = 0;
static float slider_value = 0.5f;
static bool show_panel = true;
static char last_message[256] = "Welcome to CUIFramework!";

// ============================================================================
// Guest API implementation
// ============================================================================

extern "C" {

CUI_API void cui_init(struct CUIContext* ctx) {
    frame_count = 0;
    click_count = 0;
    slider_value = 0.5f;
    show_panel = true;
    std::snprintf(last_message, sizeof(last_message), "Guest initialized at t=%.2f", ctx->total_time);
    std::printf("[Guest] Initialized! Screen: %.0fx%.0f\n", ctx->screen_width, ctx->screen_height);
}

CUI_API int cui_update(struct CUIContext* ctx) {
    frame_count++;

    // Handle input
    if (ctx->mouse_buttons & 1) { // Left click
        // Check if mouse is in a specific region
        if (ctx->mouse_x > 100 && ctx->mouse_x < 300 &&
            ctx->mouse_y > 200 && ctx->mouse_y < 240) {
            click_count++;
            std::snprintf(last_message, sizeof(last_message), "Clicked %d times!", click_count);
        }
    }

    // Update slider based on mouse
    if (ctx->mouse_buttons & 1) {
        if (ctx->mouse_x > 100 && ctx->mouse_x < 250 &&
            ctx->mouse_y > 260 && ctx->mouse_y < 280) {
            slider_value = (ctx->mouse_x - 100.0f) / 150.0f;
            if (slider_value < 0.0f) slider_value = 0.0f;
            if (slider_value > 1.0f) slider_value = 1.0f;
        }
    }

    // Handle keyboard
    if (ctx->keys_just_pressed[' ']) {
        show_panel = !show_panel;
    }

    return 0; // Return 0 to continue
}

CUI_API void cui_shutdown(struct CUIContext* ctx) {
    std::printf("[Guest] Shutdown after %d frames\n", frame_count);
}

CUI_API int cui_get_frame_count(void) {
    return frame_count;
}

CUI_API const char* cui_get_string(const char* key) {
    if (std::strcmp(key, "message") == 0) {
        return last_message;
    }
    if (std::strcmp(key, "version") == 0) {
        return "0.1.0";
    }
    return nullptr;
}

} // extern "C"
