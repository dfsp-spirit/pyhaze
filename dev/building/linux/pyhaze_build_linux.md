## Building pyhaze on Linux

This explains how to build pyhaze Linux to create source distributions. Note that distributing *binary* Linux wheels requires a `manylinux` build, which can be created with the Docker images provided by [pypa/manylinux](https://github.com/pypa/manylinux).

This document is intended for pyhaze developers only and we do not support this in any way, so please do not create issues in our repository if this does not work for you. Read the documentation of setuptools, pybind11, cmake, conda, ... instead.

This was tested on Ubuntu 22.04 LTS.


### Required Software

Before starting this, you need to install the following software:

```shell
sudo apt-get install git build-essential cmake ninja python-dev
```

You should also install `conda` if you do not have it.


### Setting up your conda build environment for pyhaze

Open shell after installing conda, and run the following commands:

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
```

### Building pyhaze

```shell
# Remove old dist/ directory if it exists from a previous build.
python setup.py sdist
```

### Uploading to PyPI

**WARNING**: *Make sure to run all tests, and that you have the correct version build (compare git commit ID), and updated with the correct version information, before uploading to PyPI.*

**WARNING**: *You may want to upload to PyPI-testing before actually uploading to PyPI, so you can spot any issues before an official release.*

This can be done by official `pyhaze` maintainers only, it requires the PyPI credentials for `pyhaze`.

```shell
twine upload dist/*  # Will ask for credentials. Only upload the sdist. Uploading the binary wheel will fail anyways, unless building on a manylinux Docker image, see notes at the very top of this document.
```

### Uploading to conda

First upload the release to PyPI (see above).

Copy the old recipe and replace the hash of the pypi archive with the one from the new PyPI release.

```shell
export PYHAZE_OLD_REL="v0.1.2"  # Replace with previous release.
export PYHAZE_NEW_REL="v0.1.3"  # Replace with the release you are preparing.
cd <pyhaze_repo>  # Replace with your path.
mkdir dev/conda_recipe/${PYHAZE_NEW_REL}
cp dev/conda_recipe/${PYHAZE_OLD_REL}/meta.yml dev/conda_recipe/${PYHAZE_NEW_REL}/
```

Now edit the commit hash.
