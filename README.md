# stdx

*My version of the C++ Standard Namespace*

---

## Table of Contents
- [Prerequisites](#prerequisites)
  - [Minimum](#minimum)
  - [Nice to have](#nice-to-have)
- [Getting Started](#getting-started)
  - [1. Clone the repository](#1-clone-the-repository)
  - [2. Configure the project](#2-configure-the-project)
  - [3. Build the project](#3-build-the-project)
  - [4. Run the test suite](#4-run-the-test-suite)
  - [5. Install the project](#5-install-the-project)
- [Developing](#developing)
- [C++ Standard Compatibility](#c-standard-compatibility)

---

## Prerequisites

### Minimum
- **CMake ≥ 3.20** – required to configure the project.
- **C++ compiler** with **C++11 or later** support:
  - Windows: Visual Studio 2022 (or newer) **or** LLVM Clang.
  - Linux: GCC **or** LLVM Clang.
  - macOS: Xcode **or** LLVM Clang.
- **Git** – needed to clone the repository.

### Nice to have
- VSCode IDE with the CMake Tools extension. This allows the use of CMake through a GUI instead of relying on the command line to build and test.
- Static analysis tools such as clang‑tidy. The `dev` and `windows-dev` CMake presets both use clang-tidy and integrate it into the build process. This is very convenient when developing!
- Ninja build system is a very fast generator and is used by the the windows-dev CMake configure preset.

---

## Getting Started
These steps will get you building and testing *stdx* on any supported platform.

### 1. Clone the repository
```shell
git clone https://github.com/andrewstef10/stdx.git
cd stdx
```

### 2. Configure the project
The repository ships with a **CMakePresets.json** file. Using the `default` preset configures CMake to use an appropriate generator and compiler for your platform:
```shell
cmake --preset default
```
> The `default` preset selects the right generator and compiler based on what is installed.

The `default` preset can be replaced with any other predefined CMake preset depending on your desired configuration and C/C++ compiler.

> To view all available presets, run `cmake --list-presets`.

### 3. Build the project
```shell
cmake --build --preset default
```

> To view all available build presets, run `cmake --build --list-presets`.

### 4. Run the test suite
To test, this project creates unit tests using Google Test (GTest). To run the unit tests, run the following:
```shell
ctest --preset default
```

> To view all available test presets, run `ctest --list-presets`.

### 5. Install the project
If you would like, after building you can install the project. To install to a default location (REPO_ROOT/install/) run the following: 
```shell
cmake --install build/default --config Debug
```
> `--config Debug` is only needed for multi config generators, but does not hurt on single config generators.

To install to a custom location of your choosing, run the following:
```shell
cmake --install build/default --config Debug --prefix "/your/custom/path"
```


> Again, `debug` can be replaced with a CMakePreset of your choosing defined in [CMakePresets.json](./CMakePresets.json)

---

## Developing
When developing, it is recommended to use the `dev` CMake preset (or `windows-dev` if developing on windows). This will use an LLVM clang compiler building for debug. These configurations also integrate clang-tidy into the build process.

## C++ Standard Compatibility

All library code must be written to be compatible with **C++11 and later**. Do not use language or library features introduced after C++11 unless they are conditionally guarded by a feature-detection macro. The project builds with CMAKE_CXX_STANDARD 20 to allow use of C++20 features in the *build system and tests*, but the *library implementation* in include/stdx/ and src/ must remain C++11-compatible.
