enable_testing()

find_package(Check)

if (CHECK_FOUND)

set(LIBS ${LIBS} ${CHECK_LIBRARIES})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CHECK_FLAGS}")
include_directories(${CHECK_INCLUDE_DIRS} ../include)

add_executable(check_cache main.c check_cache.c)
target_link_libraries(check_cache ${LIBS} cache)
add_test(test_cache ${CMAKE_CURRENT_BINARY_DIR}/check_cache)

else ()

MESSAGE("Tests disabled because Check was not found.")

endif ()
