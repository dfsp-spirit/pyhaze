// Bridge functions between pyhaze and libfs.
// Find the actualy bindings in <repo>/pytgon/pyhaze_python.cpp

#pragma once

#include <numeric>
#include <vector>

#include "libfs/libfs.hpp"

namespace pyhaze {

  static std::vector<float> smooth_pvd_nn_cpp_adj(const std::vector<std::vector<size_t>> mesh_adj, const std::vector<float> pvd, const size_t num_iter=1) {
    return fs::Mesh::smooth_pvd_nn(mesh_adj, pvd, num_iter);
  }

} // namespace pyhaze
