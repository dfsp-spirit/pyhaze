## Building pyhaze on Windows

This explains how to manually build pyhaze under Windows to distribute binary, platform-specific Windows wheels to PyPI. This is intended for pyhaze developers only and we do
not support this in any way, so please do not create issues in our repository if this does not work for you. Read the documentation of setuptools, pybind11,
cmake, conda, Visual Studio, ... instead.

This was tested on Windows 10 Professional.


### Required Software

Before starting this, you need to install the following software:

* `Microsoft Visual Studio for C++`, free when using the Community Edition. Tested with the Community Edition of 'Visual Studio for C++ 17 2022'.
* `conda`, free, we used the full 'Anaconda Distribution' (https://www.anaconda.com/products/distribution), but the Miniconda version will most likely also work.
* `git`, unless you want to install it via `conda`.

### Setting up your conda build environment for pyhaze

Open an Anaconda Command Prompt, and run the following commands:

```shell
# Clone the pyhaze repo:
git clone https://github.com/dfsp-spirit/pyhaze
cd pyhaze/

# Create a fresh conda environment and activate it:
conda create -y --name pyhaze_build
conda activate pyhaze_build

# Install required software into the build environment:
conda install cmake
conda install -y conda-build anaconda-client pip
pip install --upgrade setuptools wheel build             # just make sure we have the latest versions
pip install --upgrade twine  # Only needed if you intend to upload the Windows binary wheel to PyPI (official pyhaze maintainers only, requires credentials).
pip install pybind11
pip install dlib  # Will fail if Microsoft Visual Studio for C++ is not installed. Takes a while.
```

### Building pyhaze

```shell
# Remove old dist/ directory if it exists from a previous build.
if exist "dist\" rd /q /s "dist\"

python setup.py sdist bdist_wheel
```

### Uploading to PyPI

**WARNING**: *Make sure to run all tests, and that you have the correct version build (compare git commit ID), and updated with the correct version information, before uploading to PyPI.*

**WARNING**: *You may want to upload to PyPI-testing before actually uploading to PyPI, so you can spot any issues before an official release.*

This can be done by official `pyhaze` maintainers only, it requires the PyPI credentials for `pyhaze`.

```shell
twine upload dist/*  # Will ask for credentials. You can also upload just the Windows wheel, as the sdist will already be there from the source release done on Linux.
```
