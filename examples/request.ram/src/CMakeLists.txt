project(request.ram)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( mainrampw mainrampw mainrampw.cpp ram_approver.cpp)
add_contract( secrampw secrampw secrampw.cpp sec_base_provider.cpp)
add_contract( secrampw secramduppw secram_dup_confirm.cpp sec_base_provider.cpp)
add_contract( secrampw secramnopw secram_no_confirm.cpp sec_base_provider.cpp)
add_contract( secrampw secrampart secram_part_confirm.cpp sec_base_provider.cpp)

target_include_directories( mainrampw PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_include_directories( secrampw PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_include_directories( secramduppw PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_include_directories( secramnopw PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_include_directories( secrampart PUBLIC ${CMAKE_SOURCE_DIR}/../include )
