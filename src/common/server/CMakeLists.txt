file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/common/server/*.cpp)

include_directories(${SERVER_INCLUDE})

add_library(server STATIC
        ${sources})