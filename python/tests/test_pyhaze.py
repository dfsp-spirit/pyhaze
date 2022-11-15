
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

    num_vertices = len(np.ravel(vertices)) / 3
    assert num_vertices == 8
    assert m.num_vertices() == num_vertices

    num_faces = len(np.ravel(faces)) / 3
    assert num_faces == 12
    assert m.num_faces() == num_faces

    obj_str = m.to_obj()
    assert isinstance(obj_str, str)
    edges = m.as_edgelist()
    assert isinstance(edges, set), f"Expected set, got {type(edges)}."
    adj = m.as_adjlist(True)
    assert isinstance(adj, list)
    assert len(adj) == num_vertices



def test_smoothnn():
    vertices, faces = pyhaze.construct_cube()
    m = pyhaze.Mesh(np.ravel(vertices), np.ravel(faces))
    num_vertices = m.num_vertices()

    #mesh_adj = m.as_adjlist(True)
    pvd_data = np.arange(num_vertices, dtype=float)
    #res = pyhaze.smooth_pvd_nn(mesh_adj, pvd_data.tolist(), 5)
    res = m.smooth_pvd_nn(pvd_data.tolist(), 5, True)
    assert res.size == num_vertices
    assert isinstance(res, np.ndarray)
