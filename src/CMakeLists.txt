set(cache_src
  util.c
  impl/null.c
  impl/naive.c
)

set(libcache_ABI_MAJOR 0)
set(libcache_ABI_MINOR 0)

add_library(cache SHARED ${cache_src})
set_target_properties(cache PROPERTIES
  MACOSX_RPATH ON
  VERSION ${libcache_ABI_MAJOR}.${libcache_ABI_MINOR}
  SOVERSION ${libcache_ABI_MAJOR}
)

add_library(cache_static STATIC ${cache_src})
