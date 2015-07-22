cmake_minimum_required(VERSION 3.0)
project(libcache C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

set(libcache_VERSION_MAJOR 0)
set(libcache_VERSION_MINOR 1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_C_COMPILER_ID MATCHES "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
endif()

set(CMAKE_OSX_ARCHITECTURES x86_64)

add_subdirectory(src)
add_subdirectory(tests)
