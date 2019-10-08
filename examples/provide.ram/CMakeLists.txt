cmake_minimum_required(VERSION 3.5)

macro(add_contract_with_abi TARGET ABIFILE)
   add_executable( ${TARGET}.wasm ${ARGN} )
   configure_file(${ABIFILE} ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.abi COPYONLY)
   install (TARGETS ${TARGET}.wasm DESTINATION ${CMAKE_INSTALL_PREFIX}/${TARGET}/)
   install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.abi DESTINATION ${CMAKE_INSTALL_PREFIX}/${TARGET}/)
endmacro()

find_package(cyberway.cdt)

### Generate the wasm and abi
add_contract_with_abi(receiver.ram receiver.ram.abi ${CMAKE_CURRENT_SOURCE_DIR}/receiver.ram.cpp)

set_target_properties(receiver.ram.wasm
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

