add_executable(test1 test1.cpp)
add_executable(test2 test2.cpp)

set_target_properties(test1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set_target_properties(test2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
