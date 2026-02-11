# learn-cpp-concurrency

Build and run with CMake:

```bash
mkdir -p build
cmake -S . -B build
cmake --build build -- -j$(nproc)
./build/learn-cpp-concurrency
```

Project layout:

- [CMakeLists.txt](CMakeLists.txt) — top-level CMake configuration
- hello.cpp - Simple hello concurrent world program  
- `learn/` — scratch files for learning

To run learning files, check for exetables within the folders in ./build/learn/