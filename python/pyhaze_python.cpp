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


PYBIND11_MODULE(pyhaze, m) {
  m.doc() = "Python Bindings for pyhaze";

  py::class_<fs::Mesh>(m, "Mesh")
        .def(py::init<std::vector<float>, std::vector<int>>())
        .def("to_obj", &fs::Mesh::to_obj)
        .def("as_adjlist", &fs::Mesh::as_adjlist)
        .def("num_vertices", &fs::Mesh::num_vertices)
        .def("num_faces", &fs::Mesh::num_faces)
        //.def("smooth_pvd_nn", py::overload_cast<const std::vector<float>, const size_t, const bool>(&fs::Mesh::smooth_pvd_nn))
        .def("smooth_pvd_nn", static_cast<std::vector<float> (fs::Mesh::*)(std::vector<float>, const size_t, const bool)>(&fs::Mesh::smooth_pvd_nn))
        .def_readwrite("vertices", &fs::Mesh::vertices)
        .def_readwrite("faces", &fs::Mesh::faces)
        .def("as_edgelist", &fs::Mesh::as_edgelist);

  m.def("construct_cube", &construct_cube, "Construct simple cube mesh.");

  } // PYBIND11_MODULE

} // namespace pyhaze
