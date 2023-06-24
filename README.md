# VideoListApp

This application will display a quick and easy list of videos in the given folder and its subfolders.
Thumbnails will be provided using the Windows API for it, so it will reuse the database in your system.

It is made with Qt5 and COM interop.

## Build

It has been tested with MINGW-w64 under MSYS2 using Ninja builder

```shell
# Install dependencies
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-qt5 mingw-w64-x86_64-gcc mingw-w64-x86_64-binuntils

# Build
mkdir -p build/
cmake -DCMAKE_BUILD_TYPE=Release build/
cmake --build build/
```

Run it

```shell
build/VideoListApp.exe
```
