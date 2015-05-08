cmake_minimum_required(VERSION 2.8)
project(libcache)

set(libcache_VERSION_MAJOR 0)
set(libcache_VERSION_MINOR 1)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(src)
