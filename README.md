# pyhaze
Fast smoothing of per-vertex data on triangular meshes for Python.

## About

This package package performs smoothing of per-vertex data on [triangular meshes](https://en.wikipedia.org/wiki/Triangle_mesh). Such smoothing is typically used to reduce high-frequency noise and improve signal-to-noise ration (SNR). The algorithm is trivial, but involves nested tight loops, which are very slow in Python, so this package calls into C++ via [pybind11](https://github.com/pybind/pybind11) to achieve high performance.

![Vis](./web/pyhaze.png?raw=true "Per-vertex data on a brain mesh before (left) and after (right) smoothing.")

**Fig.1**: *Noisy per-vertex data on a brain mesh (left), and the same data after smoothing (right). White represents NA values.*

This is a Python implementation of the [haze package for R](https://github.com/dfsp-spirit/haze). The haze website offers a more detailed explanation of the motivation and use cases.

## Development state

This is work-in-progress, come back another day.
