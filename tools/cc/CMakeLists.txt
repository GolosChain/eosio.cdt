configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eosio-cc.cpp.in ${CMAKE_BINARY_DIR}/eosio-cc.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cyberway-cpp.cpp.in ${CMAKE_BINARY_DIR}/cyberway-cpp.cpp)

add_tool(eosio-cc)
add_tool(cyberway-cpp)
