#! /bin/bash

binaries=(eosio-ranlib
          eosio-ar
          eosio-objdump
          eosio-readelf
          eosio-abigen
          eosio-wasm2wast
          eosio-wast2wasm
          eosio-pp
          eosio-cc
          cyberway-cpp
          eosio-ld)

if [ -d "/usr/local/cyberway.cdt" ]; then
   printf "\tDo you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            pushd /usr/local &> /dev/null
            rm -rf cyberway.cdt
            pushd bin &> /dev/null
            for binary in ${binaries[@]}; do
               rm cyberway.${binary}
            done
            popd &> /dev/null
            pushd lib/cmake &> /dev/null
            rm -rf cyberway.cdt
            popd &> /dev/null
            break;;
         [Nn]* ) 
            printf "\tAborting uninstall\n\n"
            exit -1;;
      esac
   done
fi

if [ -d "/usr/local/eosio.wasmsdk" ]; then
   printf "\tDo you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            pushd /usr/local &> /dev/null
            rm -rf eosio.wasmsdk
            pushd bin &> /dev/null
            for binary in ${binaries[@]}; do
               rm ${binary}
            done
            popd &> /dev/null
            break;;

         [Nn]* ) 
            printf "\tAborting uninstall\n\n"
            exit -1;;
      esac
   done
fi
