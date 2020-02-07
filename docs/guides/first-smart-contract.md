 \defgroup first-smart-contract First Smart Contract
 \ingroup md_guides

# Building your first smart contract

```cpp
#include <eosio/eosio.hpp>
#include <eosio/name.hpp>

class [[eosio::contract]] hello : public eosio::contract {
    public:
        using eosio::contract::contract;

        [[eosio::action]]
        void hi(eosio::name nm) {
            eosio::print_f("Hello, %\n", nm);
        }
};
```

- Navigate to the hello folder in examples (./examples/hello).
- The hello.cpp file should be seen.
- Now run the compiler.

```sh
    $ cyberway.eosio-cpp -abigen hello.cpp -o hello.wasm
```

- Or with CMake

```sh
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
```
This will generate two files:
* The compiled binary wasm (hello.wasm)
* The generated ABI file (hello.abi)
