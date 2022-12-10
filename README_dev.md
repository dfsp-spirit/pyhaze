# Development Information for pyhaze


## Required packages for building

*Note:* You may want to do the `pip` part in a `venv` or conda environment. We use conda.

```shell
sudo apt-get install build-essential g++ cmake ninja-build  # Adapt if you're not running a Debian-like distro.
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

## Building with pyproject.toml (recommended)

```shell
cd <repo_dir>      # The pyhaze repo.
python3 -m build
```


## Building with setup.py (older pip versions)


```shell
cd <repo_dir>      # The pyhaze repo.
python3 setup.py sdist bdist_wheel
```

### FYI: Building manually with CMake

Note: This is typically not required, and the `setup.py`/`pyproject.toml` build processes explained above are preferred.

This requires `g++` with C++ 14 support or an alternative compiler kit, `cmake` and `ninja-build`.

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

IMPORTANT: You do NOT need to run these commands again. The instructions in the Section `Checking out the repo` show how to get the submodules setup correctly.

```shell
cd <repo_dir>
mkdir ext/
cd ext/
git submodule add https://github.com/catchorg/Catch2
git submodule add https://github.com/pybind/pybind11
```

Note that Catch2 is following the `v2.x` branch, see the `.gitmodules` file. This was achieved by editing that file
manually (the file gets created when running the commands listed above).


## Running the unit tests (pytest)

```shell
pip install pytest numpy
cd <repo_dir>
pytest
```

If you experience issues like `no module 'pyhaze'` errors, try `python -m pytest` instead of just `pytest`, or
set/export `PYTHONPATH=.` in your shell before running `pytest`.

## Development cycle

Note: *We currently support both `setup.py` (legacy mode for old `pip` versions) and `pyproject.toml`.*

Change into `<repo_dir>`. Then:

* Make changes to the code
* Build as explained above, i.e., run `python3 setup.py bdist_wheel`, or alternatively `python3 -m build`.
* Run `pip install -e .`
* Run the unit tests and/or an interactive Python interpreter session (`python3` or `ipython` if you have it) to test your changes.


## Building and publishing to PyPI and conda

See the information in [dev/building/] for more information.

## Documentation

Auto-generating the Python documentation is still an open issue. I cannot find any documentation or working
example of how to achieve that for a pybind11 package.

You can generate the C++ API docs with doxygen, see [instructions in the `doc/doxygen/` directory](./doc/doxygen/README_doxygen.md). However, for Python, that is not what we want. We want to document the Python API, not the C++ API.

There is a tool called `breathe`, which seems to act as a
bridge between doxygen (C++ docs) and sphinx (Python docs). It seems, with it one could write docs for functions in the C++ code (in doxygen style) and still write them addressing Python users (i.e., mention Python instead of C++
type names, etc). Then it could generate the sphinx docs based on the C++ doc strings. But I was not able to get this to work, and I spent way too much time on that already.

Given that the `pyhaze` package only exposes three functions, this is of minor concern to me right now. We simply document the API in the `README.md` file for now. I would not use pybind11 for a larger project until I find some way to get this to work though.


## Other issues

(Apart from the documentation.)


* It is not clear how to add a function written in Python to the package. This could be some simply helper function, or a function working on a more complex datastructure that you have in Python, but that does trivial stuff and therefore it is not worth it to annotate and import the large datastructure into C++, do something trivial, and pass it back. Currently the package can only include C++ functions wrapped with pybind11. Various solutions found in older StackOverflow posts also did not work, or it was unclear how to distribute them to users. Overall, it seems to me that pybind11 could use better documentation with regards to packaging the results. How to use it to wrap C++ functions is explained very well, but finding out how to ship your code to users is quite tedious.
* Adding a console script entrypoint for the package needs to be investigated. As this would be written in Python, the problem is related to the previous one.
