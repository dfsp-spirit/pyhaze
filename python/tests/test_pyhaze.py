
# Python unit tests for pyhaze.

# To install test dependencies, you will have to:
#
#     pip install pytest
#     conda install -y numpy

import pyhaze
import numpy as np


def test_construct_cube():
    res = pyhaze.construct_cube()
    assert isinstance(res, tuple)

    vertices, faces = pyhaze.construct_cube()
    assert isinstance(vertices, list)
    assert isinstance(faces, list)

    assert len(vertices) == 8
    assert len(vertices[0]) == 3
    assert isinstance(vertices[0][0], float)

    assert len(faces) == 12
    assert len(faces[0]) == 3
    assert isinstance(faces[0][0], int)


def test_init_mesh():
    vertices, faces = pyhaze.construct_cube()
    m = pyhaze.Mesh(np.ravel(vertices), np.ravel(faces))
    obj_str = m.to_obj()
    assert isinstance(obj_str, str)


def pyhaze_smoothnn_iter():
    vertices, faces = pyhaze.construct_cube()
    num_vertices = vertices.shape[0]
    mesh = pyhaze.Mesh(vertices.to_list(), faces.to_list())
    pvd_data = np.arange(num_vertices)
    res = pyhaze.smoothnn(mesh, pvd_data, num_iter=5)
    assert res.size == num_vertices
    assert isinstance(res, np.ndarray)
