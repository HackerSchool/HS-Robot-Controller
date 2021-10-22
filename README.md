# HS Robot Controller

This repository contains the controller written for the HS Robot.
It was designed in a way that allows it to be easily tested in the [Webots](https://github.com/cyberbotics/webots) simulator.

You can check the documentation [here](https://hackerschool.github.io/HS-Robot-Controller/).

## How to build

This project is built with [CMake](https://cmake.org/).

### Terminal

1. Install [CMake](https://cmake.org/).
2. Open your terminal and navigate to the controller directory.
3. Run the following commands:
  ```
  mkdir build
  cd build
  cmake .. <config>
  cmake --build .
  ```
The configuration passed to cmake here depends on which functionality you want to build the controller with.
If you want to build with Webots, add `-DUSE_WEBOTS=ON` to the command.
For bluetooth functionality, `-DUSE_BLUETOOTH=ON` must also be added.
