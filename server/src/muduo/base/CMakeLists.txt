file(GLOB_RECURSE sources ./*.cpp)

include_directories(${DNT_BASE_INCLUDE})
include_directories(${Boost_INCLUDE_DIRS})

add_library(base STATIC
        ${sources})
