#define CR_HOST
#include <cr.h>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>

// ============================================================================
// Guest state (automatically saved/restored across reloads via cr.h)
// ============================================================================

static float anim_time = 0.0f;
static int click_count = 0;
static float slider_value = 0.5f;
static bool show_panel = true;
static int frame_count = 0;

// ============================================================================
// cr_main — Guest entry point for cr.h hot reload
// ============================================================================

CR_EXPORT int cr_main(struct cr_plugin *ctx, enum cr_op operation) {
    switch (operation) {
        case CR_LOAD: {
            // Called when DLL is loaded or reloaded
            anim_time = 0.0f;
            click_count = 0;
            slider_value = 0.5f;
            show_panel = true;
            frame_count = 0;
            std::printf("[Guest] CR_LOAD: Plugin loaded/reloaded\n");
            return 0;
        }

        case CR_UNLOAD: {
            // Called before DLL is unloaded for reload
            std::printf("[Guest] CR_UNLOAD: Preparing for reload\n");
            return 0;
        }

        case CR_CLOSE: {
            // Called when plugin will close permanently
            std::printf("[Guest] CR_CLOSE: Plugin closing\n");
            return 0;
        }

        case CR_STEP:
        default: {
            // Called every frame
            frame_count++;
            anim_time += 0.016f; // ~60fps

            // Simulate some UI interaction
            // In real usage, you'd access ctx->userdata for shared state

            return 0;
        }
    }
}
