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



/** @brief Smooth per-vertex data on a mesh using nearest edge neighbor smoothing and uniform weights.
 *
 * @param mesh_adj : 2d array of integers, adjacency list representation of the faces of a mesh. The outer array has size num_vertices, the length of the inner arrays are the number of neighbors of the respective vertex.
 * @param pvd      : 1d array of floats, the per-vertex data for the mesh, with length num_vertices.
 * @param num_iter : scalar int, the number of iterations of nearest neighbor smoothing to apply.
 * @returns        : 1d numpy array of floats, the smoothed per-vertex data for the mesh, with length num_vertices.
 */
static py::array_t<float> smooth_pvd_nn(const std::vector<std::vector<size_t>> mesh_adj, const std::vector<float> pvd, const size_t num_iter) {
  auto temp_vector = smooth_pvd_nn(mesh_adj, pvd, num_iter);
  return as_pyarray(std::move(temp_vector));
}


/** @brief Return a simple cube mesh in vertex index list representation.
 * @returns : Tuple of 2D np.ndarrays, the first ndarray contains the nx3 vertex coordinates, the second ndarray contains the mx3 triangles (faces).
 */
static py::tuple construct_cube() {
  fs::Mesh m = fs::Mesh::construct_cube();
  return py::make_tuple(as_pyarray(fs::util::v2d(m.vertices, 3)), as_pyarray(fs::util::v2d(m.faces, 3)));
}



PYBIND11_MODULE(pyhaze, m) {
  m.doc() = "Python Bindings for pyhaze";
  /*
  m.def("vector_as_list", &vector_as_list,
        "Returns a vector of 16-bit ints as a Python List");
  m.def("vector_as_array", &vector_as_array,
        "Returns a vector of 16-bit ints as a NumPy array");
  m.def("vector_as_array_nocopy", &vector_as_array_nocopy,
        "Returns a vector of 16-bit ints as a NumPy array without making a "
        "copy of the data");
  */
  m.def("smooth_pvd_nn", &smooth_pvd_nn, R"mydelim(
    Smooth per-vertex data on a mesh using nearest edge neighbor smoothing and uniform weights.

    Parameters
    ----------
    mesh_adj : 2d array of integers, adjacency list representation of the faces of a mesh. The outer array has size num_vertices, the length of the inner arrays are the number of neighbors of the respective vertex.
    pvd      : 1d array of floats, the per-vertex data for the mesh, with length num_vertices.
    num_iter : scalar int, the number of iterations of nearest neighbor smoothing to apply.

    Returns
    -------
    pvd_smooth: 1d array of floats, the smoothed per-vertex data for the mesh, with length num_vertices.
)mydelim");
  };

} // namespace pyhaze
