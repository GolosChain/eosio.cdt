cmake_minimum_required(VERSION 3.5)
project(request_bw_example VERSION 1.0.0)

find_package(eosio.cdt)

### Generate the wasm and abi
add_contract_with_abi(main.pw main.pw.abi ${CMAKE_CURRENT_SOURCE_DIR}/main.pw.cpp)
add_contract_with_abi(second.pw second.pw.abi ${CMAKE_CURRENT_SOURCE_DIR}/second.pw.cpp)
add_contract_with_abi(second.fail.pw second.fail.pw.abi ${CMAKE_CURRENT_SOURCE_DIR}/second.fail.pw.cpp)
add_contract_with_abi(second.vrf.pw second.vrf.pw.abi ${CMAKE_CURRENT_SOURCE_DIR}/second.vrf.pw.cpp)


set_target_properties(main.pw.wasm
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

set_target_properties(second.pw.wasm
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

set_target_properties(second.fail.pw.wasm
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

set_target_properties(second.vrf.pw.wasm
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
