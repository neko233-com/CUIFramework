#include <cui/guest/api.h>
#include <cstdio>

// Default guest implementation
// This file is the starting point for hot-reloadable UI code

static int frame_count = 0;

extern "C" {

CUI_API void cui_init(struct CUIContext* ctx) {
    frame_count = 0;
    std::printf("[Guest] Initialized!\n");
}

CUI_API int cui_update(struct CUIContext* ctx) {
    frame_count++;
    // TODO: Build UI here
    return 0;
}

CUI_API void cui_shutdown(struct CUIContext* ctx) {
    std::printf("[Guest] Shutdown after %d frames\n", frame_count);
}

} // extern "C"
