#include <cui/hot_reload.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <filesystem>
#include <chrono>
#include <thread>

namespace cui {

// Minimal cr.h-like hot reload implementation
// This is a simplified version; the real cr.h provides more features like
// static state transfer and crash protection.

struct HotReload::Impl {
    HotReloadConfig config;
#ifdef _WIN32
    HMODULE dll_handle = nullptr;
#else
    void* dll_handle = nullptr;
#endif
    std::filesystem::file_time_type last_write_time;
    int reload_count = 0;
    bool loaded = false;

    OnInitFunc on_init;
    OnUpdateFunc on_update;
    OnShutdownFunc on_shutdown;

    bool load_dll() {
        unload_dll();

#ifdef _WIN32
        dll_handle = LoadLibraryA(config.guest_dll_path.c_str());
        if (!dll_handle) return false;
#else
        dll_handle = dlopen(config.guest_dll_path.c_str(), RTLD_NOW | RTLD_LOCAL);
        if (!dll_handle) return false;
#endif

        loaded = true;
        reload_count++;
        return true;
    }

    void unload_dll() {
        if (dll_handle) {
#ifdef _WIN32
            FreeLibrary(dll_handle);
#else
            dlclose(dll_handle);
#endif
            dll_handle = nullptr;
            loaded = false;
        }
    }

    bool check_for_changes() {
        if (config.guest_dll_path.empty()) return false;

        std::error_code ec;
        auto current_time = std::filesystem::last_write_time(config.guest_dll_path, ec);
        if (ec) return false;

        if (last_write_time != std::filesystem::file_time_type{}) {
            if (current_time > last_write_time) {
                last_write_time = current_time;
                return true;
            }
        }
        last_write_time = current_time;
        return false;
    }
};

HotReload::HotReload() : impl_(std::make_unique<Impl>()) {}
HotReload::~HotReload() { shutdown(); }

bool HotReload::init(const HotReloadConfig& config) {
    impl_->config = config;
    if (!config.guest_dll_path.empty()) {
        return impl_->load_dll();
    }
    return true;
}

void HotReload::shutdown() {
    if (impl_->on_shutdown) impl_->on_shutdown();
    impl_->unload_dll();
}

bool HotReload::update(float delta_time) {
    // Check for file changes and reload
    if (impl_->check_for_changes()) {
        impl_->unload_dll();
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Brief pause for file to finish writing
        if (!impl_->load_dll()) {
            return false;
        }
        if (impl_->on_init) impl_->on_init();
    }

    if (impl_->on_update) {
        return impl_->on_update(delta_time);
    }
    return true;
}

void HotReload::set_callbacks(OnInitFunc on_init, OnUpdateFunc on_update, OnShutdownFunc on_shutdown) {
    impl_->on_init = std::move(on_init);
    impl_->on_update = std::move(on_update);
    impl_->on_shutdown = std::move(on_shutdown);
}

bool HotReload::is_loaded() const { return impl_->loaded; }
int HotReload::reload_count() const { return impl_->reload_count; }

} // namespace cui
