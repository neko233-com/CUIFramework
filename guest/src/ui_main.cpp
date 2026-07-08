#include <cui/guest/api.h>
#include <cstdio>

// Hot-reloadable UI code
// Edit this file and save to see changes without restarting the app

static int click_count = 0;
static float slider_value = 0.5f;
static bool show_panel = true;

extern "C" {

CUI_API void cui_init(struct CUIContext* ctx) {
    click_count = 0;
    slider_value = 0.5f;
    show_panel = true;
    std::printf("[UI] Hot-reloadable UI initialized!\n");
}

CUI_API int cui_update(struct CUIContext* ctx) {
    // This function is called every frame
    // You can render using the CUIContext for input/state

    float mx = ctx->mouse_x;
    float my = ctx->mouse_y;
    bool left_click = (ctx->mouse_buttons & 1) != 0;

    // TODO: Use the declarative builder API to render UI
    // For now, this is a placeholder that prints debug info

    return 0; // Return 0 to continue
}

CUI_API void cui_shutdown(struct CUIContext* ctx) {
    std::printf("[UI] Hot-reloadable UI shutdown\n");
}

} // extern "C"
