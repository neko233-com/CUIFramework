#pragma once

#include <string>
#include <functional>
#include <memory>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <filesystem>
#include <chrono>

namespace cui {

struct HotReloadConfig {
    std::string guest_dll_path;
    bool enable_crash_protection = true;
};

class HotReload {
public:
    HotReload();
    ~HotReload();

    HotReload(const HotReload&) = delete;
    HotReload& operator=(const HotReload&) = delete;

    bool init(const HotReloadConfig& config);
    void shutdown();
    bool update(float delta_time);

    using OnInitFunc = std::function<void()>;
    using OnUpdateFunc = std::function<int(float delta_time)>;
    using OnShutdownFunc = std::function<void()>;

    void set_callbacks(OnInitFunc on_init, OnUpdateFunc on_update, OnShutdownFunc on_shutdown);

    bool is_loaded() const;
    int reload_count() const;

private:
    struct Impl {
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

        bool load_dll();
        void unload_dll();
        bool check_for_changes();
    };

    std::unique_ptr<Impl> impl_;
};

} // namespace cui
