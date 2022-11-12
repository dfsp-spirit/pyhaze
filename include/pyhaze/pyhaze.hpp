#pragma once

#include <numeric>
#include <vector>

#include "libfs/libfs.hpp"

namespace pyhaze {

/** @brief Helper function that returns a vector of given size and type
 *
 * @tparam T The type of element
 * @param size The size of the vector to return
 * @returns a vector of given size and type
 */
 /*
template <typename T> std::vector<T> make_vector(std::size_t size) {
  std::vector<T> v(size, 0);
  std::iota(v.begin(), v.end(), 0);
  return v;
}
*/

std::vector<float> smooth_pvd_nn_cpp(const std::vector<std::vector<size_t>> mesh_adj, const std::vector<float> pvd, const size_t num_iter=1) {
  return fs::Mesh::smooth_pvd_nn(mesh_adj, pvd, num_iter);
}

} // namespace pyhaze
