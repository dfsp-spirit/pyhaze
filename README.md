# pyhaze
Fast smoothing of per-vertex data on triangular meshes for Python.

## About

This package package performs smoothing of per-vertex data on triangular meshes. Such smoothing is typically used to reduce high-frequency noise and improve signal-to-noise ration (SNR). The algorithm is trivial, but involves nested tight loops, which are very slow in Python, so this package calls into C++ to achieve high performance.

This is a Python implementation of the [haze package for R](https://github.com/dfsp-spirit/haze). The haze website offers a more detailed explanation of the use case.

## Development state

This is work-in-progress, come back another day.
