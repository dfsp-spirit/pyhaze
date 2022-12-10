// Bridge functions between pyhaze and libfs.
// Find the actual bindings in <repo>/pytgon/pyhaze_python.cpp

#pragma once

#include <numeric>
#include <vector>


#include "libfs/libfs.hpp"


/// @file
///
/*! \mainpage The pyhaze C++ API documentation
 *
 * \section intro_sec Introduction
 *
 * Welcome to the C++ API documentation for pyhaze, a python package for fast smoothing of per-vertex data on triangular meshes.
 *
 * All relevant functions are in the file include/pyhaze/pyhaze.hpp and only a few utility functions are class
 * members, so the best place to start is to open the documentation for pyhaze.hpp in the Files section above.
 *
 * \subsection intro-examples A note on this API documentation
 *
 * This API doc is for the C++ interface. Note that this documentation is not what users will see in Python, as they call
 * the Python wrapper functions. People using C++ will be better off using libfs directly, as most functions in here are
 * only thin wrappers around libfs functions.
 *
 */


/// @brief The pyhaze namespace.
/// @details The pyhaze namespace. All public API functions reside in this namespace.
namespace pyhaze {

  /// @brief Smooth per-vertex data on a mesh given as adjacency list using nearest edge neighbor smoothing and uniform weights.
  /// @param mesh_adj : 2d array of integers, adjacency list representation of the faces of a mesh. The outer array has size `num_vertices`, the length of the inner arrays are the number of neighbors of the respective vertex.
  /// @param pvd      : 1d array of floats, the per-vertex data for the mesh, with length `num_vertices`.
  /// @param num_iter : scalar int, the number of iterations of nearest neighbor smoothing to apply.
  /// @param with_nan      : bool, whether to enable NAN support. On by default. Set to off for small speed boost if you are sure you have no NAN values. Ignored if `detect_nan` is `True`.
  /// @param detect_nan    : bool, whether to auto-detect the need to enable NAN support. On by default. If on, `with_nan` is ignored.
  /// @returns          the smoothed per-vertex data for the mesh, with length `num_vertices`.
  static std::vector<float> smooth_pvd_nn_cpp_adj(const std::vector<std::vector<size_t>> mesh_adj, const std::vector<float> pvd, const size_t num_iter=1, const bool with_nan=true, const bool detect_nan=true) {
    return fs::Mesh::smooth_pvd_nn(mesh_adj, pvd, num_iter, with_nan, detect_nan);
  }


  /// @brief Extend mesh neighborhoods based on mesh adjacency representation.
  /// @details This function is mainly extended to extend a source neighborhood representation (typically the mesh's `k=1` neighborhood, i.e., the adjacency list of the mesh) to a higher `k`. In a `k=3` neighborhood, the neighorhood around a source vertex includes all vertices in edge distance up to 3 from the source vertex (but not the source vertex itself).
  /// @param mesh_adj The adjacency list representation of the underlying mesh, the outer vector must have size `N` for a mesh with `N` vertices.
  /// @param extend_by the number of edges to hop to extend the neighborhoods.
  /// @return extended neighborhoods
  static std::vector<std::vector<size_t>> extend_adj(const std::vector<std::vector<size_t>> mesh_adj, const size_t extend_by=1) {
    std::vector<std::vector<size_t>> mesh_adj_ext = std::vector<std::vector<size_t>>();
    return fs::Mesh::extend_adj(mesh_adj, extend_by, mesh_adj_ext);
  }



} // namespace pyhaze
