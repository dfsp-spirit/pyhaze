// Bridge functions between pyhaze and libfs.
// Find the actualy bindings in <repo>/pytgon/pyhaze_python.cpp

#pragma once

#include <numeric>
#include <vector>


#include "libfs/libfs.hpp"




namespace pyhaze {

  static std::vector<float> smooth_pvd_nn_cpp_adj(const std::vector<std::vector<size_t>> mesh_adj, const std::vector<float> pvd, const size_t num_iter=1, const bool with_nan=true) {
    return fs::Mesh::smooth_pvd_nn(mesh_adj, pvd, num_iter, with_nan);
  }



  static std::vector<std::vector<size_t>> extend_adj(const std::vector<std::vector<size_t>> mesh_adj, const size_t extend_by=1) {
    std::vector<std::vector<size_t>> mesh_adj_ext = std::vector<std::vector<size_t>>();
    return fs::Mesh::extend_adj(mesh_adj, extend_by, mesh_adj_ext);
  }



} // namespace pyhaze
