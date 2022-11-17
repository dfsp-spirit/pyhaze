cmake_minimum_required(VERSION 3.13)
# Note: It is easy to update cmake, under Linux I would
# recommend to leave the outdated system version alone
# and download the binary from the cmake website into
# an alternate directory (e.g., /opt/cmake/ on single user
# systems, or ~/software/cmake/ on multi-user systems).

# Set a name and a version number for your project:
project(
  pyhaze
  VERSION 0.1.0
  LANGUAGES CXX)

# Initialize some default paths
include(GNUInstallDirs)

# Define the minimum C++ standard that is required
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Compilation options
set(BUILD_PYTHON
    ON
    CACHE BOOL "Enable building of Python bindings")
set(BUILD_DOCS
    ON
    CACHE BOOL "Enable building of documentation")

# compile the library
add_subdirectory(src)

if(BUILD_DOCS)
  # Add the documentation
  add_subdirectory(doc)
endif()
if(BUILD_PYTHON)
  # Add Python bindings
  include(FetchContent)
  FetchContent_Declare(
      pybind11
      GIT_REPOSITORY https://github.com/pybind/pybind11
      GIT_TAG        v2.2.3
  )

  FetchContent_GetProperties(pybind11)
  if(NOT pybind11_POPULATED)
    FetchContent_Populate(pybind11)
    add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR})
  endif()

  #add_subdirectory(ext/pybind11)
  add_subdirectory(python)
endif()

# Add an alias target for use if this project is included as a subproject in
# another project
add_library(pyhaze::pyhaze ALIAS
            pyhaze)

# Install targets and configuration
install(
  TARGETS pyhaze
  EXPORT pyhaze-config
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(
  EXPORT pyhaze-config
  NAMESPACE pyhaze::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pyhaze)

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# This prints a summary of found dependencies
include(FeatureSummary)
feature_summary(WHAT ALL)

