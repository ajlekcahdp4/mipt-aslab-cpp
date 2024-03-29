set(NET_SOURCES
  src/network.cc
)

option(USE_SPIRIT OFF)

add_executable(network ${NET_SOURCES})
target_link_libraries(network throttle circuits Boost::program_options)

if(USE_SPIRIT)

else()
  target_compile_definitions(network PUBLIC -DUSE_BISON=1)
  # Find bison and flex
  find_package(FLEX REQUIRED)
  find_package(BISON REQUIRED)

  bison_target(parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.y.cc DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/bison_network_parser.hpp)
  flex_target(scanner src/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.l.cc DEFINES_FILE)
  add_flex_bison_dependency(scanner parser)

  add_library(bison_parser STATIC ${BISON_parser_OUTPUTS} ${FLEX_scanner_OUTPUTS})
  target_include_directories(bison_parser PUBLIC include ${CMAKE_CURRENT_BINARY_DIR})

  target_link_libraries(network bison_parser)

endif()

install(TARGETS network DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(COMP_SOURCES
  src/roughly_compare.cc
)

add_executable(comp ${COMP_SOURCES})
target_link_libraries(comp PRIVATE throttle Boost::program_options)
install(TARGETS comp DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if(BASH_PROGRAM)
  add_test(NAME test.network COMMAND ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/test.sh "$<TARGET_FILE:network>" ${CMAKE_CURRENT_SOURCE_DIR} "$<TARGET_FILE:comp>")
endif()
