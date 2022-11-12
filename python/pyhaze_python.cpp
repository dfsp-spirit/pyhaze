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

/** @brief Return a vector as a Python List
 *
 * @param size The size of the vector to return
 * @returns the vector as a Python List
 */
 /*
static std::vector<short> vector_as_list(std::size_t size) {
  return make_vector<short>(size);
}
*/

/** @brief Return a vector as a NumPy array
 *
 * Makes a copy of an existing vector of data
 *
 * @param size The size of the vector to return
 * @returns the vector as a NumPy array
 */
/*static py::array_t<short> vector_as_array(std::size_t size) {
  auto temp_vector = make_vector<short>(size);
  return py::array(size, temp_vector.data());
}
*/

/** @brief Return a vector as a NumPy array
 *
 * Moves the contents of an existing vector of data
 *
 * @param size The size of the vector to return
 * @returns the vector as a NumPy array
 */
/*static py::array_t<short> vector_as_array_nocopy(std::size_t size) {
  auto temp_vector = make_vector<short>(size);
  return as_pyarray(std::move(temp_vector));
}
*/

static py::array_t<float> smooth_pvd_nn(const std::vector<std::vector<size_t>> mesh_adj, const std::vector<float> pvd, const size_t num_iter=1) {
  auto temp_vector = smooth_pvd_nn(mesh_adj, pvd, num_iter);
  return as_pyarray(std::move(temp_vector));
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
  m.def("smooth_pvd_nn", &smooth_pvd_nn,
    "Smooth per-vertex data on a mesh using nearest edge neighbor smoothing and uniform weights.");
  };

} // namespace pyhaze
