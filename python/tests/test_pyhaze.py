
# Python unit tests for pyhaze.

# To install test dependencies, you will have to:
#
#     pip install pytest
#     conda install -y numpy

import pyhaze
import numpy as np
import pytest


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


def test_init_mesh_1d():
    vertices, faces = pyhaze.construct_cube()
    m = pyhaze.Mesh(np.ravel(vertices), np.ravel(faces))

    num_vertices = len(np.ravel(vertices)) / 3
    assert num_vertices == 8
    assert m.num_vertices() == num_vertices

    num_faces = len(np.ravel(faces)) / 3
    assert num_faces == 12
    assert m.num_faces() == num_faces


def test_init_mesh_2d():
    vertices, faces = pyhaze.construct_cube()
    m = pyhaze.Mesh(vertices, faces)

    num_vertices = len(np.ravel(vertices)) / 3
    assert num_vertices == 8
    assert m.num_vertices() == num_vertices

    num_faces = len(np.ravel(faces)) / 3
    assert num_faces == 12
    assert m.num_faces() == num_faces


def test_mesh_member_functions():
    vertices, faces = pyhaze.construct_cube()
    m = pyhaze.Mesh(np.ravel(vertices), np.ravel(faces))

    obj_str = m.to_obj()
    assert isinstance(obj_str, str)
    edges = m.as_edgelist()
    assert isinstance(edges, set), f"Expected set, got {type(edges)}."
    adj = m.as_adjlist(True)
    assert isinstance(adj, list)
    assert len(adj) == m.num_vertices()


def test_smooth_pvd():
    vertices, faces = pyhaze.construct_cube()
    m = pyhaze.Mesh(vertices, faces)
    num_vertices = m.num_vertices()

    pvd_data = np.arange(num_vertices, dtype=float)
    res = pyhaze.smooth_pvd(vertices, faces, pvd_data.tolist(), 5)
    assert res.size == num_vertices
    assert isinstance(res, np.ndarray)


def test_smooth_pvd_adj():
    vertices, faces = pyhaze.construct_cube()
    m = pyhaze.Mesh(vertices, faces)
    num_vertices = m.num_vertices()

    pvd_data = np.arange(num_vertices, dtype=float)
    mesh_adj = m.as_adjlist(True)
    res = pyhaze.smooth_pvd_adj(mesh_adj, pvd_data.tolist(), 5)
    assert res.size == num_vertices
    assert isinstance(res, np.ndarray)


def test_smooth_pvd_adj_with_igl():
    igl = pytest.importorskip('igl')  # Test will be skipped if Python package 'igl' is not installed.
    vertices, faces = pyhaze.construct_cube()
    m = pyhaze.Mesh(vertices, faces)
    num_vertices = m.num_vertices()

    pvd_data = np.arange(num_vertices, dtype=float)
    faces_igl = np.array(faces).reshape(-1, 3)
    mesh_adj = igl.adjacency_list(faces_igl)
    res = pyhaze.smooth_pvd_adj(mesh_adj, pvd_data.tolist(), 5)
    assert res.size == num_vertices
    assert isinstance(res, np.ndarray)
