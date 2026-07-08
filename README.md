# CUIFramework

C++23 Cross-platform UI Framework with declarative layout and cr.h hot reload.

## Features

- **C++23** — Modern C++ with `std::format`, `std::print`, `std::expected`
- **Cross-platform** — Windows, macOS, Linux
- **Declarative UI** — Clay-like API for describing layouts
- **Hot Reload** — Edit UI code and see changes instantly via cr.h
- **SDL2 + OpenGL** — Reliable cross-platform rendering
- **Multi-architecture** — x86, x64, ARM64 support

## Quick Start

### Prerequisites

- CMake 3.28+
- C++23 capable compiler (MSVC 2022+, GCC 14+, Clang 17+)
- Git

### Build

```bash
# Clone with submodules
git clone --recursive https://github.com/neko233-com/CUIFramework.git
cd CUIFramework

# Windows (x64)
cmake --preset win-x64-release
cmake --build --preset win-x64-release

# Windows (x86/32-bit)
cmake --preset win-x86-release
cmake --build --preset win-x86-release

# Linux (x64)
cmake --preset linux-x64-release
cmake --build --preset linux-x64-release

# Linux (ARM64)
cmake --preset linux-arm64-release
cmake --build --preset linux-arm64-release

# macOS (ARM64)
cmake --preset macos-arm64-release
cmake --build --preset macos-arm64-release
```

### Build Scripts

```bash
# Windows
scripts\build.bat win-x64-release
scripts\build.bat win-x86-release

# Linux/macOS
chmod +x scripts/build.sh
scripts/build.sh linux-x64-release
scripts/build.sh linux-arm64-release
```

## Project Structure

```
CUIFramework/
├── core/          # Core layer (window, renderer, input) — never reloaded
├── ui/            # Declarative UI engine
├── guest/         # Hot-reloadable UI code (compiled to .dll/.so)
├── examples/      # Example applications
├── extern/        # Third-party dependencies
└── scripts/       # Build scripts
```

## Hot Reload

The framework uses [cr.h](https://github.com/fungos/cr) for hot reload:

1. UI code is compiled to a shared library (`cui_guest.dll`/`.so`)
2. The host app watches for file changes
3. When you edit `guest/src/ui_main.cpp` and save, the DLL reloads
4. Your UI updates without restarting the application

## Architecture

```
┌─────────────────────────────────┐
│         Host (cui_host.exe)     │  ← Core layer, never reloaded
│  ┌───────────┐ ┌─────────────┐ │
│  │ SDL2      │ │ OpenGL      │ │
│  └───────────┘ └─────────────┘ │
│  ┌───────────────────────────┐ │
│  │     cr.h Host Runtime     │ │
│  └───────────┬───────────────┘ │
└──────────────┼─────────────────┘
               │ loads/reloads
┌──────────────┼─────────────────┐
│         Guest (cui_guest.dll)  │  ← Hot-reloadable UI code
│  ┌───────────▼───────────────┐ │
│  │  CR_EXPORT cr_main()      │ │
│  │  Build UI declaratively   │ │
│  └───────────────────────────┘ │
└─────────────────────────────────┘
```

## API Example

```cpp
#include <cui/app.h>
#include <cui/ui/builder.h>

int main() {
    cui::AppConfig config;
    config.title = "My App";
    config.width = 1280;
    config.height = 720;

    return cui::run_app(config,
        [](cui::App& app) { /* setup */ },
        [](cui::App& app, float dt) { /* update */ },
        [](cui::App& app) {
            cui::ui::Builder builder(app.renderer(),
                                   (float)app.window().width(),
                                   (float)app.window().height());

            builder.begin_layout(cui::FlexDirection::Column, 16, cui::Padding(32));
            {
                builder.text("Hello, CUIFramework!", 24, cui::Colors::White);
                builder.button("Click Me");
            }
            builder.end_layout();
        }
    );
}
```

## License

MIT License
