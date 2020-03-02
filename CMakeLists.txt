cmake_minimum_required(VERSION 3.5)
project(cyberway_cdt)

set(CDT_CCACHE_BUILD OFF CACHE BOOL "Set to ON for a ccache enabled build")
if(CDT_CCACHE_BUILD)
   find_program(SCCACHE_FOUND sccache)
   if (SCCACHE_FOUND)
      message(STATUS "Using sccache")
      set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE sccache)
   else()
      find_program(CCACHE_FOUND ccache)
      if (CCACHE_FOUND)
         set(CDT_CCACHE_PARAMS "CCACHE_CPP2=yes CCACHE_HASHDIR=yes" CACHE STRING "Parameters to pass through to ccache")
         set(CDT_CCACHE_PROGRAM "${CDT_CCACHE_PARAMS} ${CCACHE_FOUND}")
         message(STATUS "Using ccache: ${CDT_CCACHE_PROGRAM}")
         set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CDT_CCACHE_PROGRAM})
      endif()
   endif()
endif()


set(VERSION_MAJOR 1)
set(VERSION_MINOR 6)
set(VERSION_PATCH 3)
#set(VERSION_SUFFIX rc2)

if (VERSION_SUFFIX)
   set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_SUFFIX}")
else()
   set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
endif()

set(CDT_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/cyberway.cdt)

include(GNUInstallDirs)

include(modules/ClangExternalProject.txt)
include(modules/ToolsExternalProject.txt)

set(WASM_SDK_BUILD true)

### Configure the CyberwayWasmToolchain.cmakes
set(CDT_ROOT_DIR ${CMAKE_BINARY_DIR})
configure_file(${CMAKE_SOURCE_DIR}/modules/eosio.cdt-config.cmake ${CMAKE_BINARY_DIR}/lib/cmake/cyberway.cdt/cyberway.cdt-config.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioCDTMacros.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/cyberway.cdt/CyberwayCDTMacros.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/cyberway.cdt/CyberwayWasmToolchain.cmake @ONLY)

set(CDT_ROOT_DIR ${CDT_INSTALL_PREFIX})
configure_file(${CMAKE_SOURCE_DIR}/modules/eosio.cdt-config.cmake ${CMAKE_BINARY_DIR}/modules/cyberway.cdt-config.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioCDTMacros.cmake.in ${CMAKE_BINARY_DIR}/modules/CyberwayCDTMacros.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/modules/CyberwayWasmToolchain.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/imports/eosio.imports.in ${CMAKE_BINARY_DIR}/eosio.imports COPYONLY)

set(CDT_ROOT_DIR "_PREFIX_")
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioCDTMacros.cmake.in ${CMAKE_BINARY_DIR}/modules/CyberwayCDTMacrosPackage.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/modules/CyberwayWasmToolchainPackage.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/eosio.cdt-config.cmake ${CMAKE_BINARY_DIR}/modules/cyberway.cdt-config.cmake.package @ONLY)

include(modules/LibrariesExternalProject.txt)

# section for package construction
set(VENDOR "CyberWay.Core")
set(PROJECT_NAME "cyberway.cdt")
set(DESC "Toolchain and supporting tools for the CyberWay platform")
set(URL "https://github.com/cyberway/cyberway.cdt")
set(EMAIL "support@cyberway.io")
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_package.sh.in ${CMAKE_BINARY_DIR}/packages/generate_package.sh @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_deb.sh ${CMAKE_BINARY_DIR}/packages/generate_deb.sh COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_bottle.sh ${CMAKE_BINARY_DIR}/packages/generate_bottle.sh COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_rpm.sh ${CMAKE_BINARY_DIR}/packages/generate_rpm.sh COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_tarball.sh ${CMAKE_BINARY_DIR}/packages/generate_tarball.sh COPYONLY)

# add licenses
configure_file(${CMAKE_SOURCE_DIR}/eosio_llvm/LICENSE.TXT ${CMAKE_BINARY_DIR}/licenses/llvm.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/boost/boost.license ${CMAKE_BINARY_DIR}/licenses/boost.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/tools/external/wabt/LICENSE ${CMAKE_BINARY_DIR}/licenses/wabt.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/tools/jsoncons/LICENSE ${CMAKE_BINARY_DIR}/licenses/jsoncons.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/eosio.cdt.license ${CMAKE_BINARY_DIR}/licenses/eosio.cdt.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/LICENSE ${CMAKE_BINARY_DIR}/licenses/cyberway.cdt.license COPYONLY)

include(modules/InstallCDT.cmake)

include(modules/TestsExternalProject.txt)

include(CTest)
enable_testing()
add_subdirectory(tests)
