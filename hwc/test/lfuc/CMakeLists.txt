set(LFUDAC_SOURCES
  src/lfuc.cc
)

add_executable(lfuc ${LFUDAC_SOURCES})
target_link_libraries(lfuc caches)