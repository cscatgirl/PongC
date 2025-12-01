# Pong

Pong

## About RCade

This game is built for [RCade](https://rcade.recurse.com), a custom arcade cabinet at The Recurse Center. Learn more about the project at [github.com/fcjr/RCade](https://github.com/fcjr/RCade).

## Prerequisites

- [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) - The Emscripten SDK for compiling C/C++ to WebAssembly
- Make (usually pre-installed on Linux/macOS, use [MinGW](http://mingw.org/) on Windows)
- Python 3 (for local development server)

### Installing Emscripten

```bash
# Clone the emsdk repository
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install and activate the latest SDK
./emsdk install latest
./emsdk activate latest

# Add to PATH (add this to your ~/.bashrc or ~/.zshrc)
source ./emsdk_env.sh
```

## Getting Started

Start the development server:

```bash
make serve
```

This compiles the C++ code to WebAssembly and serves it at http://localhost:8000 with hot reloading.

## Building

### Development Build
```bash
make dev
```

Builds with debug symbols and assertions enabled. Output goes to `build/`.

### Production Build
```bash
make build
```

Builds an optimized production bundle. Output goes to `dist/` and is ready for deployment.

## Project Structure

```
├── src/
│   └── main.cpp          # Game entry point
├── sdk/cpp/              # RCade C++ SDK
│   ├── include/rcade/    # SDK headers
│   └── src/              # SDK implementation
├── index.html            # HTML entry
└── Makefile              # Build configuration
```

## Using the RCade C++ SDK

This template includes the RCade C++ SDK for easy canvas and input handling:

```cpp
#include <rcade/rcade.h>
#include <emscripten.h>

rcade::Canvas* canvas;
rcade::Input* input;

void gameLoop() {
    // Clear screen
    canvas->clear("#1a1a2e");

    // Get input
    const auto& p1 = input->getPlayer1();
    if (p1.UP) {
        // Player 1 pressed UP
    }

    // Draw
    canvas->fillRect(100, 100, 50, 50, "#ff0000");
    canvas->fillText("Hello!", 168, 131, "24px monospace", "#fff", "center");
}

int main() {
    canvas = new rcade::Canvas(336, 262);
    input = new rcade::Input();
    emscripten_set_main_loop(gameLoop, 60, 1);
    return 0;
}
```

See `docs\sdk\cpp_sdk` for full API documentation.

## Deployment

First, create a new repository on GitHub:

1. Go to [github.com/new](https://github.com/new)
2. Create a new repository (can be public or private)
3. **Don't** initialize it with a README, .gitignore, or license

Then connect your local project and push:

```bash
git remote add origin git@github.com:YOUR_USERNAME/YOUR_REPO.git
git push -u origin main
```

The included GitHub Actions workflow will automatically deploy to RCade.

---

Made with <3 at [The Recurse Center](https://recurse.com)
