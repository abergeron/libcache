set(cache_src
  impl/null.c
  impl/naive.c
  impl/lru.c
  impl/twoq.c
)

set(libcache_ABI_MAJOR 0)
set(libcache_ABI_MINOR 2)

add_library(cache SHARED ${cache_src})
set_target_properties(cache PROPERTIES
  MACOSX_RPATH ON
  VERSION ${libcache_ABI_MAJOR}.${libcache_ABI_MINOR}
  SOVERSION ${libcache_ABI_MAJOR}
)

add_library(cache_static STATIC ${cache_src})

INSTALL(TARGETS cache cache_static
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
