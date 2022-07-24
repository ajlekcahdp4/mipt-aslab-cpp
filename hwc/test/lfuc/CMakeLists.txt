set(LFUDAC_SOURCES
  src/lfuc.cc
)

add_executable(lfuc ${LFUDAC_SOURCES})
target_link_libraries(lfuc caches Boost::program_options)

install(TARGETS lfuc DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if(BASH_PROGRAM)
  add_test(NAME test.lfu COMMAND ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/test.sh "$<TARGET_FILE:lfuc>" ${CMAKE_CURRENT_SOURCE_DIR})
endif()