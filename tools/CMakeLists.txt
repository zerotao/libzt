install(PROGRAMS zt_mkc zt_hdr DESTINATION bin)

if(NOT WIN32)
    configure_file(libzt.pc.cmake libzt.pc @ONLY)
    configure_file(libzt_gc.pc.cmake libzt_gc.pc @ONLY)
    configure_file(libzt_unit.pc.cmake libzt_unit.pc @ONLY)

# CMAKE_CURRENT_BINARY_DIR only needed here because it is a generated file (I think)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libzt.pc
                  ${CMAKE_CURRENT_BINARY_DIR}/libzt_gc.pc
                  ${CMAKE_CURRENT_BINARY_DIR}/libzt_unit.pc
                  DESTINATION lib/pkgconfig)
endif(NOT WIN32)
