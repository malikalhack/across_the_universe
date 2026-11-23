# Across the Universe

A fullscreen starfield application written in C++ with SDL2.

## Linux build

Install the compiler, CMake, and SDL2 development package:

```sh
sudo apt install build-essential cmake libsdl2-dev
```

Configure and build:

```sh
cmake -S . -B build
cmake --build build
```

Run the application with:

```sh
./build/AcrossTheUniverse
```

Press `Esc` to quit. The application opens in desktop fullscreen mode.

The same CMake project can be used on Windows and macOS with SDL2 development
files installed and available to CMake.

The existing `.sln` and `.vcxproj` files belong to the previous Windows-only
version. Use this CMake project on all platforms; Visual Studio can open the
source directory as a CMake project.