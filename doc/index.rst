------
pyhaze
------

.. toctree::
   :maxdepth: 2
   :caption: Contents:

About
=====

This package package performs smoothing of per-vertex data on `triangular meshes <https://en.wikipedia.org/wiki/Triangle_mesh>`_.
Such smoothing is typically used to reduce high-frequency noise and improve signal-to-noise ration (SNR).
The algorithm for iterative nearest-neighbor smoothing is trivial, but involves nested tight loops, which are very slow
in Python, so this package calls into C++ via `pybind11 <https://github.com/pybind/pybind11>`_
to achieve high performance.


Project Information
====================

Please see our GitHub repo at `github.com/dfspspirit/pyhaze <https://github.com/dfspspirit/pyhaze>`_.

