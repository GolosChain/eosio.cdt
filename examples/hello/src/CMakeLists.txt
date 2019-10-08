project(hello)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(cyberway.cdt)

add_contract( hello hello hello.cpp )
target_include_directories( hello PUBLIC ${CMAKE_SOURCE_DIR}/../include )
