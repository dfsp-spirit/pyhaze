# Development Information for pyhaze

## Required packages

```shell
sudo apt-get install cmake ninja-build
```

## Building with CMake

```shell
cd <repo>
mkdir build/
cmake -S . -B build/
cd build
ninja
```

## Packaging for PyPi

```shell
pip install pybind11
python3 -m pip install --upgrade build
cd <repo>
python3 -m build
```



