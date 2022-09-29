set(QUERIES_SOURCES
  src/determinant.cc
)

add_executable(determinant ${QUERIES_SOURCES})
target_link_libraries(determinant throttle)
if(Boost_FOUND)
  target_link_libraries(determinant Boost::program_options)
endif()

install(TARGETS determinant DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# if(BASH_PROGRAM)
#   add_test(NAME test.determinant COMMAND ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/test.sh "$<TARGET_FILE:determinant>" ${CMAKE_CURRENT_SOURCE_DIR})
# endif()