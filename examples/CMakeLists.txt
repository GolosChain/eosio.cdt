macro(add_contract_with_abi TARGET ABIFILE)
   add_executable( ${TARGET}.wasm ${ARGN} )
   configure_file(${ABIFILE} ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.abi COPYONLY)
   install (TARGETS ${TARGET}.wasm DESTINATION ${CMAKE_INSTALL_PREFIX}/${TARGET}/)
   install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.abi DESTINATION ${CMAKE_INSTALL_PREFIX}/${TARGET}/)
endmacro()

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

add_subdirectory(request.bw)
add_subdirectory(hello)

