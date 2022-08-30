set(LIBRARY_SOURCES
  src/temp.cc
)

add_library(throttle ${LIBRARY_SOURCES})
target_include_directories(throttle PUBLIC include)

set(UNIT_TEST_SOURCES
  test/main.cc
  test/test_equal.cc
  test/test_vec.cc
  test/test_plane.cc
)

if (ENABLE_GTEST)
  add_executable(unit_test ${UNIT_TEST_SOURCES})
  target_include_directories(unit_test PRIVATE src include)
  target_link_libraries(unit_test throttle ${GTEST_BOTH_LIBRARIES})
  gtest_discover_tests(unit_test)
endif()