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

// Guest API — these functions are called by the host
// The guest DLL must implement all of them.

struct CUIContext {
    float delta_time;
    float screen_width;
    float screen_height;
    float mouse_x;
    float mouse_y;
    int mouse_buttons; // Bitmask: bit 0 = left, bit 1 = right, bit 2 = middle
};

// Called once when the guest DLL is loaded
CUI_API void cui_init(struct CUIContext* ctx);

// Called every frame. Returns 0 to continue, non-zero to exit.
CUI_API int cui_update(struct CUIContext* ctx);

// Called once when the guest DLL is about to be unloaded
CUI_API void cui_shutdown(struct CUIContext* ctx);

#ifdef __cplusplus
}
#endif
