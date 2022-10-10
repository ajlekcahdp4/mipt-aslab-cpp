set(QUERIES_SOURCES
  src/determinant.cc
)

add_executable(determinant ${QUERIES_SOURCES})
target_link_libraries(determinant throttle Boost::program_options)

install(TARGETS determinant DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(COMP_SOURCES
  src/roughly_compare.cc
)

add_executable(comp ${COMP_SOURCES})
target_link_libraries(comp throttle)
target_link_libraries(comp Boost::program_options)
install(TARGETS comp DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if(BASH_PROGRAM)
  add_test(NAME test.determinant COMMAND ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/test.sh "$<TARGET_FILE:determinant>" ${CMAKE_CURRENT_SOURCE_DIR} "$<TARGET_FILE:comp>")
endif()