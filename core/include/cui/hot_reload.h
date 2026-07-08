#pragma once

#include <string>
#include <functional>

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
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace cui
