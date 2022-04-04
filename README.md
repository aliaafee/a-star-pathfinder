# A* Search Pathfinder

Pathfinder using A* search algorithm, implemented in C++ and packaged as a python library using PyBind11.

![Demo Animation](screenshots/01.png?raw=true)

## Required Dependencies

pybind11 (https://github.com/pybind/pybind11) is required.
```
git submodule init
```

## Build

```
mkdir build
cd build
cmake ..
make
```

## Examples

```
examples/pathfinder_test.py
```
Simple example demonstrating how to use the library.

```
examples/pathfinder_test_gui.py
```
GUI example demonstrating the library. Requires pygame. Mouse click to set end point (orange dot). "S" key to search from current mouse position to end point. "A" to add obstacle. "B" to remove obstacle.

