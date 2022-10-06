set(EXEC_SOURCES
  src/closest.cc
)

add_executable(closest ${EXEC_SOURCES})
target_link_libraries(closest throttle range-v3::range-v3)
if(Boost_FOUND)
  target_link_libraries(closest Boost::program_options)
endif()

install(TARGETS closest DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# if(BASH_PROGRAM)
#   add_test(NAME test.closest COMMAND ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/test.sh "$<TARGET_FILE:closest>" ${CMAKE_CURRENT_SOURCE_DIR})
# endif()