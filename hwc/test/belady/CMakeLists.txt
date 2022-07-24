set(BELADY_SOURCES
  src/belady.cc
)

add_executable(belady ${BELADY_SOURCES})
target_link_libraries(belady caches)

install(TARGETS belady DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if(BASH_PROGRAM)
  add_test(NAME test.belady COMMAND ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/test.sh "$<TARGET_FILE:belady>" ${CMAKE_CURRENT_SOURCE_DIR})
endif()