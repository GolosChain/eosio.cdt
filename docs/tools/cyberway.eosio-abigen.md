 \defgroup cyberway-eosio-abigen CyberWay-Eosio Abigen
 \ingroup md_tools

# cyberway.eosio-abigen tool

> **This tool is deprecated, please use *cyberway.eosio-cpp* for generation of your ABIs.**  
To generate an ABI with *cyberway.eosio-abigen*, only requires that you give the main ".cpp" file to compile and the output filename *--output* and generating against the contract name *--contract*.

Example:

```bash
$ cyberway.eosio-abigen hello.cpp --contract=hello --output=hello.abi
```

This will generate one ABI file (hello.abi):

```
USAGE: cyberway.eosio-abigen [options] <source0> [... <sourceN>]

OPTIONS:

Generic Options:

  -help                      - Display available options (-help-hidden for more)
  -help-list                 - Display list of available options (-help-list-hidden for more)
  -version                   - Display the version of this program

cyberway.eosio-abigen:
generates an ABI from C++ project input

  -extra-arg=<string>        - Additional argument to append to the compiler command line
  -extra-arg-before=<string> - Additional argument to prepend to the compiler command line
  -output=<string>           - Set the output filename and fullpath
  -p=<string>                - Build path
```

