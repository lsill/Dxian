add_executable(test1 test1.cpp)
add_executable(test2 test2.cpp)

set_target_properties(test1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set_target_properties(test2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

target_link_libraries(test1
        PRIVATE
        base
        net)

target_link_libraries(test2
        PRIVATE
        base
        net)
include_directories(${Boost_INCLUDE_DIRS})
target_include_directories(test1 PUBLIC "${CMAKE_SOURCE_DIR}/muduo/base/include")
target_include_directories(test1 PUBLIC "${CMAKE_SOURCE_DIR}/muduo/net/include")

target_include_directories(test2 PUBLIC "${CMAKE_SOURCE_DIR}/muduo/base/include")
target_include_directories(test2 PUBLIC "${CMAKE_SOURCE_DIR}/muduo/net/include")
