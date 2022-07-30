set(LIBRARY_SOURCES
  src/rb_tree_ranged.cc
)

add_library(throttle ${LIBRARY_SOURCES})
target_include_directories(throttle PUBLIC include src)