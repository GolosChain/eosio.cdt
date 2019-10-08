cmake_minimum_required(VERSION 3.5)

macro(add_contract_with_abi TARGET ABIFILE)
   add_executable( ${TARGET}.wasm ${ARGN} )
   configure_file(${ABIFILE} ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.abi COPYONLY)
   install (TARGETS ${TARGET}.wasm DESTINATION ${CMAKE_INSTALL_PREFIX}/${TARGET}/)
   install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.abi DESTINATION ${CMAKE_INSTALL_PREFIX}/${TARGET}/)
endmacro()

find_package(cyberway.cdt)

if (FAIL_CASE)
    set(SECOND_PROVODER_NAME second.fail.pw)
    add_compile_definitions(FAIL_CASE)
elseif(VERIFIED_CASE)
    set(SECOND_PROVODER_NAME second.vrf.pw)
    add_compile_definitions(VERIFIED_CASE)
else(FAIL_CASE)
    set(SECOND_PROVODER_NAME second.pw)
endif(FAIL_CASE)

### Generate the wasm and abi
add_contract_with_abi(main.pw provider.abi ${CMAKE_CURRENT_SOURCE_DIR}/main.pw.cpp)
add_contract_with_abi(${SECOND_PROVODER_NAME} provider.abi ${CMAKE_CURRENT_SOURCE_DIR}/second.pw.cpp)

set_target_properties(main.pw.wasm
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

set_target_properties(${SECOND_PROVODER_NAME}.wasm
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
