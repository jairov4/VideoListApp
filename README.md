# VideoListApp

This application will display a quick and easy list of videos in the given folder and its subfolders.
Thumbnails will be provided using the Windows API for it, so it will reuse the database in your system.
You can realtime filter the list just typing in the search text box.

It is made with Qt5 and COM interop.

## Why I created it?

- I could use VideoLAN but it is good player not a browser.
- I could use Kodi or (insert here your favorite Home Theater software):
  - Kodi is optimized for single video playback (instead of double click! bam! new video window in parallel)
  - Kodi is optimized for AFK usage (like remote control or TV based)
  - Kodi will require a separated thumbnail indexing (here you reuse the Windows database)
  - Kodi is an always rendering app, which is ok, but I wanted a lighter app do the job.
- I could use the Windows Explorer. Right, it provides the thumbnails and the search box. But...
  - For some reason the Windows Explorer is so slow loading their own cache.
  - Windows Explorer search feature will use heavier full-text search engine. 
    I dont want to accidentally search text in Word, Excel files or .nfo files or .torrent files.

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
