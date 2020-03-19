# CyberWay.CDT

## Contract Development Toolkit based on EOSIO.CDT

### Version : 1.6.3

CyberWay.CDT is a toolchain for WebAssembly (WASM) and set of tools to facilitate contract writing for the CyberWay platform.  In addition to being a general purpose WebAssembly toolchain, [CyberWay](https://github.com/cyberway/cyberway) specific optimizations are available to support building CyberWay smart contracts.  This toolchain is built around [Clang 7](https://github.com/eosio/llvm), which means that CyberWay.CDT has the most currently available optimizations and analyses from LLVM, but as the WASM target is still considered experimental, some optimizations are not available or incomplete.


**If you have previously installed CyberWay.CDT, run the *uninstall* script (it is in the directory where you cloned cyberway.cdt) before downloading and using the binary releases.**

### Guided Installation (Building from Scratch)

```sh
 $ git clone --recursive https://github.com/cyberway/cyberway.cdt
 $ cd cyberway.cdt
 $ ./build.sh
 $ sudo ./install.sh
```

### Installed Tools

* cyberway.eosio-cpp
* cyberway.eosio-cc
* cyberway.eosio-ld
* cyberway.eosio-init
* cyberway.eosio-abigen
* cyberway.eosio-abidiff
* cyberway.eosio-pp
* cyberway.eosio-wasm2wast
* cyberway.eosio-wast2wasm
* cyberway.eosio-ranlib
* cyberway.eosio-ar
* cyberway.eosio-objdump
* cyberway.eosio-readelf

## Contributing

[Contributing Guide](https://github.com/cyberway/cyberway.cdt/blob/master/CONTRIBUTING.md#"Contributing to cyberway.cdt")

[Code of Conduct](https://github.com/cyberway/cyberway.cdt/blob/master/CONTRIBUTING.md#conduct)

## License

See [LICENSE](https://github.com/cyberway/cyberway.cdt/blob/master/LICENSE) for copyright and license terms.

## Important

All repositories and other materials are provided subject to the terms of this [IMPORTANT](https://github.com/cyberway/cyberway.cdt/blob/master/IMPORTANT.md#"Important Notice") notice and you must familiarize yourself with its terms. The notice contains important information, limitations and restrictions relating to our software, publications, trademarks, third-party resources, and forward-looking statements. By accessing any of our repositories and other materials, you accept and agree to the terms of the notice.
