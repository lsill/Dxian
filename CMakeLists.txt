cmake_minimum_required(VERSION 3.16...3.22)

message(STATUS "CMake Version:${CMAKE_VERSION}")

project(DXian)

# set(CMAKE_DISABLE_SOURCE_CHANGES ON) 防止构建过程修改源代码。
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON) # 禁止在源代码目录内进行构建

#CMAKE_SKIP_BUILD_RPATH 0 会在构建（编译）阶段使用 RPATH
# 构建期间生成的可执行文件或动态库会包含用于定位依赖的动态库的 RPATH
set(CMAKE_SKIP_BUILD_RPATH 0)
# CMAKE_BUILD_WITH_INSTALL_RPATH 0 CMake 在构建时不会使用安装时的 RPATH。
# 构建阶段和安装阶段使用的 RPATH 是不同的。在安装后，可执行文件或动态库将使用不同的 RPATH 来定位依赖的动态库。
set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

add_subdirectory(src/common)