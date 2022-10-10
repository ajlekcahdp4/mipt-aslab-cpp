find_package(Boost REQUIRED COMPONENTS program_options)

if(Boost_FOUND)
  add_compile_definitions(BOOST_FOUND__)
endif()

add_subdirectory(determinant)

enable_testing()