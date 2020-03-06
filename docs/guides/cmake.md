 \defgroup md_guides Guides

 \defgroup cmake Cmake
 \ingroup md_guides

# CMake

## CMake Configuration

Compiling an EOSIO smart contract with CMake requires a CMake file. The new `cyberway.eosio-init` tool can be used to generate the directory structure stub .hpp/.cpp files and subsequent cmake files. Also, the template `CMakeLists.txt` in the examples folder is a good boilerplate for manual usage.

For example:

In **CMakeLists.txt**:

```
cmake_minimum_required(VERSION 3.5)
project(test_example VERSION 1.0.0)

find_package(cyberway.cdt)

add_contract( test test test.cpp )
```


In **test.cpp**:

```cpp
#include <eosio/eosio.hpp>
using namespace eosio;

CONTRACT test : public eosio::contract {
public:
   using contract::contract;

   ACTION testact( name test ) {
   }
};

EOSIO_DISPATCH( test, (testact) )
```

## CMake Macros
- `add_contract` is used to build smart contract and generate an ABI. The first parameter is the contract name, the second is the cmake target name, and the rest are the CPP files needed to build the contract.
- (new for native tester) `add_native_library` and `add_native_executable` CMake macros have been added (these are a drop in replacement for add_library and add_executable).
