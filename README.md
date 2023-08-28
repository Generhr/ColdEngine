# ColdEngine

[![Continuous Integration](https://github.com/Onimuru/ColdEngine/actions/workflows/continuous-integration.yml/badge.svg)](https://github.com/Onimuru/ColdEngine/actions/workflows/continuous-integration.yml)
[![codecov](https://codecov.io/gh/Onimuru/ColdEngine/branch/main/graph/badge.svg)](https://codecov.io/gh/Onimuru/ColdEngine)
[![Documentation](https://svgshare.com/i/vaA.svg)](https://onimuru.github.io/ColdEngine/)
[![License](https://camo.githubusercontent.com/890acbdcb87868b382af9a4b1fac507b9659d9bf/68747470733a2f2f696d672e736869656c64732e696f2f62616467652f6c6963656e73652d4d49542d626c75652e737667)](./LICENSE)

## Building

First, clone this repo:

```shell
git clone --recursive https://github.com/Onimuru/ColdEngine
```

- Executable

```shell
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
cd bin/Release
./<your project name>
```

- Unit testing

```shell
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug --target unit_tests
ctest -C Debug -VV
```

- Documentation

```shell
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug --target doxygen
```
