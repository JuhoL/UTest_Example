# UTest_Example
An example project for unit testing embedded systems. You can use this project as a template for establishing unit testing for your project.

## Folders
* **Sources** - Source files for the unit test examples.
* **TestingUtils** - Utilities for unit tests, such as Catch2 and FFF headers.
* **UTest** - A folder for unit test build scripts.

## Building

### Prerequisites
* C and C++ compiler
* [CMake](https://cmake.org/) - CMake for generating the build environment.
* [Catch2](https://github.com/catchorg/Catch2) - A unit test framework (headers are included in this project).
* [FFF](https://github.com/meekrosoft/fff) - A simple single-header C mock framework (header is included in this project).

The easiest way to setup the environment is to use Linux or [Linux sub-system in Windows 10](https://www.windowscentral.com/install-windows-subsystem-linux-windows-10). In Linux all you need is to install build-essential package and CMake.

### Building
Easiest way is to run `./build.sh` in UTest directory.

If you build in Windows, you can build manually with CMake:
* Navigate to UTest directory
* Initialize CMake project: `cmake .`
* Build the project: `cmake --build .`
* Run the tests: `ctest --output-on-failure`
