set(cache_src
  util.c
  impl/null.c
  impl/naive.c
)

add_library(cache SHARED ${cache_src})
set_target_properties(cache PROPERTIES
  MACOSX_RPATH ON
)

add_library(cache_static STATIC ${cache_src})
