cmake_minimum_required(VERSION 2.8)
project(libcache C)

set(libcache_VERSION_MAJOR 0)
set(libcache_VERSION_MINOR 1)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_C_COMPILER_ID MATCHES "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
endif()

add_subdirectory(src)
