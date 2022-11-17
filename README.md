# pyhaze
Fast smoothing of per-vertex data on triangular meshes for Python.

## About

This package package performs smoothing of per-vertex data on [triangular meshes](https://en.wikipedia.org/wiki/Triangle_mesh). Such smoothing is typically used to reduce high-frequency noise and improve signal-to-noise ration (SNR). The algorithm for iterative nearest-neighbor smoothing is trivial, but involves nested tight loops, which are very slow in Python, so this package calls into C++ via [pybind11](https://github.com/pybind/pybind11) to achieve high performance.

![Figure 1, Showing a brain mesh with an overlay, before and after smoothing.](./web/pyhaze.png?raw=true "Per-vertex data on a brain mesh before (left) and after (right) smoothing.")

**Fig.1**: *Noisy per-vertex data on a brain mesh (left), and the same data after smoothing (right). White represents NA values.*

This is a Python implementation of the [haze package for R](https://github.com/dfsp-spirit/haze). The haze website offers a more detailed explanation of the motivation and use cases.

## Usage

### Example 1: Smooth data on a mesh given as a vertex index list

Here is a simple example using the `pyhaze.smooth_pvd` function.

```python
import pyhaze
import numpy as np

vertices, faces = pyhaze.construct_cube()

pvd_data = np.arange(len(vertices), dtype=float)
smoothed_data = pyhaze.smooth_pvd(vertices, faces, pvd_data.tolist(), 5)
```

A note on the mesh representation used, so you can replace the `vertices` and `faces` with your own triangular mesh:

* `vertices` is a list of lists of `float`, with dimension `N, 3` for `N` vertices. So the outer list has length `N`. The 3 columns (length of all inner lists) are the x,y,z coordinates for each vertex.
* `faces` is a list of lists of `int`, with dimension `M, 3` for `M` faces. So the outer list has length `M`. The 3 columns (length of all inner lists) are the 3 vertices (given as indices into `vertices`) making up the respective triangular face.


### Example 2: Smooth data on a mesh given as an adjacency list

For very large meshes, it pays off to pre-compute the adjacency list of the mesh with a fast method, such as with the `igl` Python package, which provides Python bindings for libigl, and use the `pyhaze.smooth_pvd_adj` function.

```python
import pyhaze
import numpy as np
import igl

vertices, faces = pyhaze.construct_cube()

pvd_data = np.arange(len(vertices), dtype=float)
faces_igl = np.array(faces).reshape(-1, 3)
mesh_adj = igl.adjacency_list(faces_igl)  # Compute adjacency list with igl.
res = pyhaze.smooth_pvd_adj(mesh_adj, pvd_data.tolist(), 5)
```

See the [unit tests](./python/tests/test_pyhaze.py) for more examples.
