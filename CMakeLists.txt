cmake_minimum_required(VERSION 3.5)

message(STATUS "CMake Version:${CMAKE_VERSION}")

project(DXian)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON) # 生成compile_commands.json 查看编译单元所执行指令
SET(BUILD_SHARED_LIBS ON)  # 没有指定生成什么库 默认静态
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON) # 必须支持20

# 检查操作系统
if(APPLE)
    message(STATUS "On Apple systems, Clang is recommended.")
    # 设置 Clang 相关的特定标志或配置
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
elseif(UNIX AND NOT APPLE)
    message(STATUS "On Linux systems, GCC is recommended.")
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
endif()

# TO_CMAKE_PATH 将提供的路径转换成CMake风格路径
file(TO_CMAKE_PATH ${PROJECT_BINARY_DIR}/CMakeLists.txt, PATH_TO_CMAKE_TXT)
if (EXISTS "${PATH_TO_CMAKE_TXT}")
    message(FATAL_ERROR "Run CMake from a build subdirectory! \"mkdir build ; cd build ; cmake .. \" \
    Some junk files were created in this folder (CMakeCache.txt, CMakeFiles); you should delete those.")
endif ()
message(${PATH_TO_CMAKE_TXT})

# set(CMAKE_DISABLE_SOURCE_CHANGES ON) 防止构建过程修改源代码。
#set(CMAKE_DISABLE_IN_SOURCE_BUILD ON) # 禁止在源代码目录内进行构建

#CMAKE_SKIP_BUILD_RPATH 0 会在构建（编译）阶段使用 RPATH
# 构建期间生成的可执行文件或动态库会包含用于定位依赖的动态库的 RPATH
#set(CMAKE_SKIP_BUILD_RPATH 0)
# CMAKE_BUILD_WITH_INSTALL_RPATH 0 CMake 在构建时不会使用安装时的 RPATH。
# 构建阶段和安装阶段使用的 RPATH 是不同的。在安装后，可执行文件或动态库将使用不同的 RPATH 来定位依赖的动态库。
#set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)

#set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# CMAKE_ARCHIVE_OUTPUT_DIRECTORY 静态库输出目录
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# CMAKE_LIBRARY_OUTPUT_DIRECTORY 动态库输出目录
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# CMAKE_RUNTIME_OUTPUT_DIRECTORY 可执行文件输出目录s
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


SET(SERVER_INCLUDE ${PROJECT_SOURCE_DIR}/src/common/server/include)

add_subdirectory(src/common/server)
add_subdirectory(src/test)