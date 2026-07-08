#pragma once

#ifdef _WIN32
    #ifdef CUI_GUEST_EXPORTS
        #define CUI_API __declspec(dllexport)
    #else
        #define CUI_API __declspec(dllimport)
    #endif
#else
    #define CUI_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Guest API — these functions are called by the host
// ============================================================================

struct CUIContext {
    // Time
    float delta_time;
    float total_time;

    // Screen
    float screen_width;
    float screen_height;

    // Input
    float mouse_x;
    float mouse_y;
    int mouse_buttons;      // Bitmask: bit 0 = left, bit 1 = right, bit 2 = middle
    int mouse_wheel;

    // Keyboard
    int keys_pressed[256];  // Bitmask of currently pressed keys
    int keys_just_pressed[256]; // Keys pressed this frame

    // User data (POD, safe across reloads)
    void* user_data;
};

// ============================================================================
// Guest callbacks — implement these in your hot-reloadable code
// ============================================================================

// Called once when the guest DLL is loaded
CUI_API void cui_init(struct CUIContext* ctx);

// Called every frame. Returns 0 to continue, non-zero to exit.
CUI_API int cui_update(struct CUIContext* ctx);

// Called once when the guest DLL is about to be unloaded
CUI_API void cui_shutdown(struct CUIContext* ctx);

// ============================================================================
// Helper functions (optional)
// ============================================================================

// Get the current frame count (useful for animations)
CUI_API int cui_get_frame_count(void);

// Get a string from the host (e.g., window title, user settings)
CUI_API const char* cui_get_string(const char* key);

#ifdef __cplusplus
}
#endif
