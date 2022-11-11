# Development Information for pyhaze

## On the stuff in `ext/`

The `Catch2` and `pybind11` directories/links are git submodules. They were added via:

```shell
git submodule add https://github.com/catchorg/Catch2
git submodule add https://github.com/pybind/pybind11
```

TODO: We should consider re-adding at a specific commit or tag for better reproducibility:

* `pybind11` commit used: `1f04cc7`
* `Catch2` commit used: `41990e0`


## Required packages for building

*Note:* You may want to do the `pip` part in a `venv`.

```shell
sudo apt-get install g++ cmake ninja-build
pip install pybind11
pip install --upgrade build
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
cd <repo>
python3 setup.py sdist
python3 setup.py bdist_wheel
```



