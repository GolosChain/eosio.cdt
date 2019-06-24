# CyberWay.CDT (Contract Development Toolkit)
## Version : 1.6.1

CyberWay.CDT is a toolchain for WebAssembly (WASM) and set of tools to facilitate contract writing for the CyberWay platform.  In addition to being a general purpose WebAssembly toolchain, [CyberWay](https://github.com/GolosChain/cyberway) specific optimizations are available to support building CyberWay smart contracts.  This toolchain is built around [Clang 7](https://github.com/eosio/llvm), which means that CyberWay.CDT has the most currently available optimizations and analyses from LLVM, but as the WASM target is still considered experimental, some optimizations are not available or incomplete.


### Guided Installation (Building from Scratch)
```sh
$ git clone --recursive https://github.com/GolosChain/cyberway.cdt
$ cd cyberway.cdt
$ ./build.sh
$ sudo ./install.sh
```

### Installed Tools
---
* eosio-cpp
* eosio-cc
* eosio-ld
* eosio-init
* eosio-abidiff
* eosio-wasm2wast
* eosio-wast2wasm
* eosio-ranlib
* eosio-ar
* eosio-objdump
* eosio-readelf
