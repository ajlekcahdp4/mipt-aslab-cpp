set(LFUDAC_SOURCES
    src/lfudac.cc
)

add_executable(lfudac ${LFUDAC_SOURCES})
target_link_libraries(lfudac caches)