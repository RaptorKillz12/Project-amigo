# Project-amigo Native Engine

This directory contains the first native C++ runtime foundation for Project-amigo.

## Current capabilities

- Builds a reusable C++17 core library.
- Builds a Windows x64 command-line inspection utility.
- Builds an Android ARM64 JNI shared library.
- Scans a user-selected data directory and reports file, directory, and byte counts.

## Build on desktop

```sh
cmake -S engine -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Inspect a user-selected folder:

```sh
amigo_inspect --inspect-data /path/to/SambaDeAmigoData
```

## Android

The GitHub Actions workflow cross-compiles `libproject_amigo.so` for `arm64-v8a`.
The JNI class expected by this bridge is
`com.projectamigo.runtime.AmigoNative`.

## Important status

This is an engine foundation, **not yet a playable Samba de Amigo port**.
It does not load Unity/iOS executables, emulate iOS APIs, interpret IL2CPP,
decrypt content, or implement the game's graphics, audio, charts, or gameplay.
The next work is to establish a documented, lawful game-data format and add
individual compatibility subsystems using files the user is authorized to use.
Do not distribute the original game's executable, proprietary assets, or other
copyrighted game data in this repository.
