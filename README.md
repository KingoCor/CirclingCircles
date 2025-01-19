You can play [here](https://kingocor.github.io/CirclingCircles/)

# Build
## Desktop
```bash
mkdir build && cd build
cmake ..
cmake --build .
```
## Web
```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=<YOUR PATH>/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DPLATFORM=Web
cmake --build .
```
