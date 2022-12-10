#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "pyhaze/pyhaze.hpp"

#include <algorithm>
#include <vector>

namespace py = pybind11;

/// @brief Internal helper function to avoid making a copy when returning a py::array_t.
/// @note Not exposed to Python. Source: https://github.com/pybind/pybind11/issues/1042#issuecomment-642215028
/// @internal
/// @author author: https://github.com/YannickJadoul
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
   * @param with_nan      : bool, whether to enable NAN support. On by default. Set to off for small speed boost if you are sure you have no NAN values. Ignored if `detect_nan` is `True`.
   * @param detect_nan    : bool, whether to auto-detect the need to enable NAN support. On by default. If on, `with_nan` is ignored.
   * @returns        : 1d numpy array of floats, the smoothed per-vertex data for the mesh, with length num_vertices.
   */
  static py::array_t<float> smooth_pvd_adj(const std::vector<std::vector<size_t>> mesh_adj, const std::vector<float> pvd, const size_t num_iter, const bool with_nan=true, const bool detect_nan=true) {
    auto temp_vector = smooth_pvd_nn_cpp_adj(mesh_adj, pvd, num_iter, with_nan, detect_nan);
    return as_pyarray(std::move(temp_vector));
  }

    /** @brief Smooth per-vertex data on a mesh given as a vertex index list using nearest edge neighbor smoothing and uniform weights.
   *
   * @param mesh_vertices : 2d array of floats, the vertex coordinates for N vertices as an Nx3 matrix. Can be empty, as vertex coordinates are not required for nearest neighbor smoothing. In here to prevent calling this with an adjacency list by accident.
   * @param mesh_faces    : 2d array of size_t, the faces for N vertices as an Nx3 matrix of vertex indices.
   * @param pvd           : 1d array of floats, the per-vertex data for the mesh, with length num_vertices.
   * @param num_iter      : scalar int, the number of iterations of nearest neighbor smoothing to apply.
   * @param via_matrix    : whether the mesh adjacency list should be computed via a matrix (faster, requires more memory) as opposed to an edge list (slower, requires less memory).
   * @param with_nan      : bool, whether to enable NAN support. On by default. Set to off for small speed boost if you are sure you have no NAN values. Ignored if `detect_nan` is `True`.
   * @param detect_nan    : bool, whether to auto-detect the need to enable NAN support. On by default. If on, `with_nan` is ignored.
   * @returns             : 1d numpy array of floats, the smoothed per-vertex data for the mesh, with length `num_vertices`.
   */
  static py::array_t<float> smooth_pvd(const std::vector<std::vector<float>> mesh_vertices, const std::vector<std::vector<int32_t>> mesh_faces, const std::vector<float> pvd, const size_t num_iter = 1, const bool via_matrix = true, const bool with_nan=true, const bool detect_nan=true) {
    fs::Mesh m = fs::Mesh(mesh_vertices, mesh_faces);
    auto mesh_adj = m.as_adjlist(via_matrix);
    auto temp_vector = smooth_pvd_nn_cpp_adj(mesh_adj, pvd, num_iter, with_nan, detect_nan);
    return as_pyarray(std::move(temp_vector));
  }

  // See doSomethingWithArray function in
  // https://developer.lsst.io/v/u-ktl-debug-fix/coding/python_wrappers_for_cpp_with_pybind11.html
  // for an example of a C++ function that takes a np.ndarray as argument.


  PYBIND11_MODULE(pyhaze, m) {
    m.doc() = "Python Bindings for pyhaze";

    py::class_<fs::Mesh>(m, "Mesh", "A vertex index list representation of a triangle mesh.")
          .def(py::init<std::vector<float>, std::vector<int>>())
          .def(py::init<std::vector<std::vector<float>>, std::vector<std::vector<int>>>())
          .def("to_obj", &fs::Mesh::to_obj, "Get string representation of the mesh instance in Wavefront Object (OBJ) format, plain text version.")
          .def("as_adjlist", &fs::Mesh::as_adjlist, "Compute adjacency list representation of the mesh instance.")
          .def("num_vertices", &fs::Mesh::num_vertices, "Return the number of vertices of the mesh instance.")
          .def("num_faces", &fs::Mesh::num_faces, "Return the number of faces (triangles) of the mesh instance.")
          .def_readwrite("vertices", &fs::Mesh::vertices, "Access the vertices of the mesh instance.")
          .def_readwrite("faces", &fs::Mesh::faces, "Access the faces of the mesh instance.")
          .def("as_edgelist", &fs::Mesh::as_edgelist, "Compute edge list representation of the mesh instance.");

    m.def("construct_cube", &construct_cube, "Construct simple cube mesh.");
    m.def("smooth_pvd_adj", &smooth_pvd_adj, "Smooth data on mesh given as adjacency list. Allows you to use fast methods to compute mesh adjacency, like `igl.adjacency_list()` from the 'igl' Python package.", py::arg("mesh_adj"), py::arg("pvd"), py::arg("num_iter") = 1, py::arg("with_nan") = true, py::arg("detect_nan") = true);
    m.def("smooth_pvd", &smooth_pvd, "Smooth data on mesh given as vertex index list.", py::arg("mesh_vertices"), py::arg("mesh_faces"), py::arg("pvd"), py::arg("num_iter") = 1, py::arg("via_matrix") = true, py::arg("with_nan") = true, py::arg("detect_nan") = true);
    m.def("extend_adj", &extend_adj, "Extend neighborhoods by k iterations of edge hopping.", py::arg("mesh_adj"), py::arg("extend_by") = 1);
  } // PYBIND11_MODULE

} // namespace pyhaze
