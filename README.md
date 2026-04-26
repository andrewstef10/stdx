# stdx
My version of C++ Standard Namespace


## Prerequisites
* **CMake ≥ 3.20**
* A C++ compiler:
  * Windows: Visual Studio 2022 or newer
  * Linux: GCC or Clang
  * macOS: Xcode / Clang
* Git (to clone the repository)


## Command-Line Build & Test Instructions

### Clone the repo
```shell
git clone https://github.com/yourusername/stdx.git
cd stdx
```

### Create Build Directory
```shell
mkdir build
cd build
```

### Configure Project with CMake
```shell
cmake ..
```

### Build

* Windows
    ```shell
    cmake --build . --config Debug
    cmake --build . --config Release
    ```

### Run tests
* Windows
    ```shell
    ctest -C Debug
    ctest -C Release
    ```