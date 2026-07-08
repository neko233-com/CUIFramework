#define CR_HOST
#include <cr.h>
#include <cui/hot_reload.h>
#include <cstdio>

namespace cui {

struct HotReload::Impl {
    cr_plugin ctx;
    HotReloadConfig config;
    bool loaded = false;
    int reload_count = 0;

    bool open(const std::string& path) {
        config.guest_dll_path = path;
        if (cr_plugin_open(ctx, path.c_str())) {
            loaded = true;
            reload_count++;
            std::printf("[HotReload] Plugin loaded: %s\n", path.c_str());
            return true;
        }
        std::printf("[HotReload] Failed to load: %s\n", path.c_str());
        return false;
    }

    void close() {
        if (loaded) {
            cr_plugin_close(ctx);
            loaded = false;
        }
    }

    bool update() {
        if (!loaded) return false;
        int result = cr_plugin_update(ctx);
        return result == 0;
    }
};

HotReload::HotReload() : impl_(std::make_unique<Impl>()) {}
HotReload::~HotReload() { shutdown(); }

bool HotReload::init(const HotReloadConfig& config) {
    impl_->config = config;
    if (!config.guest_dll_path.empty()) {
        return impl_->open(config.guest_dll_path);
    }
    return true;
}

void HotReload::shutdown() {
    impl_->close();
}

bool HotReload::update(float delta_time) {
    return impl_->update();
}

void HotReload::set_callbacks(OnInitFunc, OnUpdateFunc, OnShutdownFunc) {
    // cr.h handles callbacks via cr_main
}

bool HotReload::is_loaded() const { return impl_->loaded; }
int HotReload::reload_count() const { return impl_->reload_count; }

} // namespace cui
