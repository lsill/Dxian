file(GLOB_RECURSE sources ./*.cpp)

include_directories(${DNT_NET_INCLUDE})
include_directories(${Boost_INCLUDE_DIRS})

add_library(net STATIC
        ${sources})

target_link_libraries(net
        PRIVATE
         base)

target_include_directories(net PUBLIC "${CMAKE_SOURCE_DIR}/muduo/base/include")

