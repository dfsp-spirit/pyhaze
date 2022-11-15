pybind11_add_module(pyhaze_python
                    pyhaze_python.cpp)
target_link_libraries(pyhaze_python PUBLIC pyhaze)
set_target_properties(pyhaze_python
                      PROPERTIES OUTPUT_NAME pyhaze)

