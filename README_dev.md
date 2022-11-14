# Development Information for pyhaze


## On the stuff in `ext/`

The `Catch2` and `pybind11` directories/links are git submodules. They were added via the following commands.

Note that you do NOT need to run these commands again. The repo cloning instructions below show how to get the submodules.

```shell
git submodule add https://github.com/catchorg/Catch2
git submodule add https://github.com/pybind/pybind11
```

Note that Catch2 is following the `v2.x` branch, see the `.gitmodules` file.

## Required packages for building

*Note:* You may want to do the `pip` part in a `venv`.

```shell
sudo apt-get install g++ cmake ninja-build  # Adapt if you're not running a Debian-like distro.
pip install pybind11
pip install --upgrade build
```

## Checking out the repo

```shell
cd
git clone https://github.com/dfsp-spirit/pyhaze
cd pyhaze  # Called <repo_dir> from now on.
git submodule init  # Required to init stuff in ext/
```


## Building manually with CMake


```shell
cd <repo_dir>  # The pyhaze repo.
mkdir build/
cmake -S . -B build/
cd build
ninja
```

*Note: If your cmake version is older than the required one, it is easy to update (or install a later version into another directory) from the [cmake website](https://cmake.org/download).*

## Running the dev version

First build it, then:

```shell
python
```

and:

```python
import pyhaze
...
```

## Packaging for PyPi


```shell
cd <repo_dir>  # The pyhaze repo.
python3 setup.py sdist
python3 setup.py bdist_wheel
```

