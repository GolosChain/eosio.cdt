 \defgroup cyberway-eosio-ld CyberWay-Eosio Ld
 \ingroup md_tools

# cyberway.eosio-ld tool

The cyberway.eosio-ld tool is the custom web assembly linker.

```
USAGE: cyberway.eosio-ld [options] <input file> ...

OPTIONS:

Generic Options:

  -help             - Display available options (-help-hidden for more)
  -help-list        - Display list of available options (-help-list-hidden for more)
  -version          - Display the version of this program

ld options:

  -L=<string>       - Add directory to library search path
  -fasm             - Assemble file for x86-64
  -fnative          - Compile and link for x86-64
  -fno-cfl-aa       - Disable CFL Alias Analysis
  -fno-lto          - Disable LTO
  -fno-post-pass    - Don't run post processing pass
  -fno-stack-first  - Don't set the stack first in memory
  -stack-size       - Specifies the maximum stack size for the contract
  -fuse-main        - Use main as entry
  -l=<string>       - Root name of library to link
  -lto-opt=<string> - LTO Optimization level (O0-O3)
  -o=<string>       - Write output to <file>
```
