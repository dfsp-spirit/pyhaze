# Development Information for pyhaze


## Required packages for building

*Note:* You may want to do the `pip` part in a `venv` or conda env.

```shell
sudo apt-get install g++ cmake ninja-build  # Adapt if you're not running a Debian-like distro.
pip install pybind11  # TODO: Is this required, now that we use the git submodule? But it does not hurt.
pip install --upgrade build
```

## Checking out the repo

```shell
cd
git clone https://github.com/dfsp-spirit/pyhaze
cd pyhaze  # Called <repo_dir> from now on.
git submodule update --init  # Required to init stuff in ext/
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

## Building with setup.py


```shell
cd <repo_dir>  # The pyhaze repo.
python3 setup.py sdist
python3 setup.py bdist_wheel
```

### FYI: Building manually with CMake

Note: This is typically not required, and the setup.py build process explained above is preferred.

It is just FYI. This requires `g++` with C++ 14 support or an alternative compiler kit, `cmake` and `ninja-build`.

```shell
cd <repo_dir>  # The pyhaze repo.
mkdir build/
cmake -S . -B build/
cd build
ninja
```

*Note: If your cmake version is older than the required one, it is easy to update (or install a later version into another directory) from the [cmake website](https://cmake.org/download).*



## On the origins of the stuff in `ext/`

The `Catch2` and `pybind11` directories/links are git submodules. They were added via the following commands.

Note that you do NOT need to run these commands again. The instructions in the Section `Checking out the repo` show how to get the submodules setup correctly.

```shell
git submodule add https://github.com/catchorg/Catch2
git submodule add https://github.com/pybind/pybind11
```

Note that Catch2 is following the `v2.x` branch, see the `.gitmodules` file.


## Running the unit tests (pytest)

```shell
pip install pytest numpy
cd <repo_dir>
pytest
```

## Development cycle

Change into `<repo_dir>`. Then:

* Make changes to the code
* Run `python3 setup.py bdist_wheel`
* Run `pip install -e .`
* Run the unit tests and/or an interactive Python interpreter session (`python3` or `ipython` if you have it) to test your changes.

