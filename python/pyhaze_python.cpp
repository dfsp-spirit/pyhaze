#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "pyhaze/pyhaze.hpp"

namespace py = pybind11;

// helper function to avoid making a copy when returning a py::array_t
// author: https://github.com/YannickJadoul
// source: https://github.com/pybind/pybind11/issues/1042#issuecomment-642215028
template <typename Sequence>
inline py::array_t<typename Sequence::value_type> as_pyarray(Sequence &&seq) {
  auto size = seq.size();
  auto data = seq.data();
  std::unique_ptr<Sequence> seq_ptr =
      std::make_unique<Sequence>(std::move(seq));
  auto capsule = py::capsule(seq_ptr.get(), [](void *p) {
    std::unique_ptr<Sequence>(reinterpret_cast<Sequence *>(p));
  });
  seq_ptr.release();
  return py::array(size, data, capsule);
}

namespace pyhaze {


/** @brief Return a simple cube mesh in vertex index list representation.
 * @returns : Tuple of 2D np.ndarrays, the first ndarray contains the nx3 vertex coordinates, the second ndarray contains the mx3 triangles (faces).
 */
static py::tuple construct_cube() {
  fs::Mesh m = fs::Mesh::construct_cube();
  return py::make_tuple(std::move(fs::util::v2d(m.vertices, 3)), std::move(fs::util::v2d(m.faces, 3)));
}


/** @brief Smooth per-vertex data on a mesh given as adjacency list using nearest edge neighbor smoothing and uniform weights.
 *
 * @param mesh_adj : 2d array of integers, adjacency list representation of the faces of a mesh. The outer array has size num_vertices, the length of the inner arrays are the number of neighbors of the respective vertex.
 * @param pvd      : 1d array of floats, the per-vertex data for the mesh, with length num_vertices.
 * @param num_iter : scalar int, the number of iterations of nearest neighbor smoothing to apply.
 * @returns        : 1d numpy array of floats, the smoothed per-vertex data for the mesh, with length num_vertices.
 */
static py::array_t<float> smooth_pvd_adj(const std::vector<std::vector<size_t>> mesh_adj, const std::vector<float> pvd, const size_t num_iter) {
  auto temp_vector = smooth_pvd_nn_cpp_adj(mesh_adj, pvd, num_iter);
  return as_pyarray(std::move(temp_vector));
}

/** @brief Smooth per-vertex data on a mesh given as a vertex index list using nearest edge neighbor smoothing and uniform weights.
 *
 * @param mesh_vertices : 2d array of floats, the vertex coordinates for N vertices as an Nx3 matrix. Can be empty, as vertex coordinates are not required for nearest neighbor smoothing. In here to prevent calling this with an adjacency list by accident.
 * @param mesh_faces    : 2d array of size_t, the faces for N vertices as an Nx3 matrix of vertex indices.
 * @param pvd           : 1d array of floats, the per-vertex data for the mesh, with length num_vertices.
 * @param num_iter      : scalar int, the number of iterations of nearest neighbor smoothing to apply.
 * @param via_matrix    : whether the mesh adjacency list should be computed via a matrix (faster, requires more memory) as opposed to an edge list (slower, requires less memory).
 * @returns             : 1d numpy array of floats, the smoothed per-vertex data for the mesh, with length num_vertices.
 */
static py::array_t<float> smooth_pvd(const std::vector<std::vector<float>> mesh_vertices, const std::vector<std::vector<int32_t>> mesh_faces, const std::vector<float> pvd, const size_t num_iter = 1, const bool via_matrix = true) {
  fs::Mesh m = fs::Mesh(mesh_vertices, mesh_faces);
  auto mesh_adj = m.as_adjlist(via_matrix);
  auto temp_vector = smooth_pvd_nn_cpp_adj(mesh_adj, pvd, num_iter);
  return as_pyarray(std::move(temp_vector));
}

// See doSomethingWithArray function in
// https://developer.lsst.io/v/u-ktl-debug-fix/coding/python_wrappers_for_cpp_with_pybind11.html
// for an example of a C++ function that takes a np.ndarray as argument.


PYBIND11_MODULE(pyhaze, m) {
  m.doc() = "Python Bindings for pyhaze";

  py::class_<fs::Mesh>(m, "Mesh")
        .def(py::init<std::vector<float>, std::vector<int>>())
        .def(py::init<std::vector<std::vector<float>>, std::vector<std::vector<int>>>())
        .def("to_obj", &fs::Mesh::to_obj)
        .def("as_adjlist", &fs::Mesh::as_adjlist)
        .def("num_vertices", &fs::Mesh::num_vertices)
        .def("num_faces", &fs::Mesh::num_faces)
        .def_readwrite("vertices", &fs::Mesh::vertices)
        .def_readwrite("faces", &fs::Mesh::faces)
        .def("as_edgelist", &fs::Mesh::as_edgelist);

  m.def("construct_cube", &construct_cube, "Construct simple cube mesh.");
  m.def("smooth_pvd_adj", &smooth_pvd_adj, "Smooth data on mesh given as adjacency list. Allows you to use fast methods to computes mesh adjacency, like igl.adjacency_list from the 'igl' Python package.", py::arg("mesh_adj"), py::arg("pvd"), py::arg("num_iter") = 1);
  m.def("smooth_pvd", &smooth_pvd, "Smooth data on mesh given as vertex index list.", py::arg("mesh_vertices"), py::arg("mesh_faces"), py::arg("pvd"), py::arg("num_iter") = 1, py::arg("via_matrix") = true);

  } // PYBIND11_MODULE

} // namespace pyhaze
