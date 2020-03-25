 \defgroup md_cdt_specifics CyberWay.CDT Specifics

 \defgroup cdt-specifics List of Differences
 \ingroup md_cdt_specifics

This section lists main differences between CyberWay.CDT and EOSIO.CDT.

## 1. No Ricardian contracts in CyberWay.CDT, including:
 - CMakeLists.txt - Ricardian contracts code was removed from contract configuration
 - docs/guides/abi-generator-attributes.md - Ricardian contracts and clauses were removed from ABI
 - docs/guides/cmake.md - Ricardian contracts code was removed from building of API file
 - modules/EosioCDTMacros.cmake.in - no modules related to Ricardian contracts
 - tools/abigen/eosio-abigen.cpp.in - no tools related to Ricardian contracts

Detailed information can be found [here][1].
 
## 2. The multi_index logic was re-designed using database functions (chaindb_*)

List of newly developed functions:
 - chaindb_lower_bound
 - chaindb_upper_bound
 - chaindb_find
 - chaindb_end
 - chaindb_clone
 - chaindb_close
 - chaindb_current
 - chaindb_next
 - chaindb_prev
 - chaindb_datasize
 - chaindb_data
 - chaindb_available_primary_key
 - chaindb_insert
 - chaindb_update
 - chaindb_delete

See also the [Multi Index Table][2] section.
 
## 3. Checking the "weak" rights

New functions `weak_require_auth` and `weak_require_auth2` were developed and implemented in the `eosio.imports.in` file for checking the "weak" rights. These functions do not interrupt process and return a result in form of a boolean instead of throwing an exception.  

Detailed information related to these functions can be found in [Action][3] section.

## 4. Supporting domain/user names
 
New functions were developed and implemented in the `eosio.imports.in` file for processing domain/user names. The new functions list:
 - is_domain
 - is_username
 - get_domain_owner
 - resolve_domain
 - resolve_username

Detailed information related to these functions can be found in [Domain C++ API][4] section.
 
## 5. New logic was developed and implemented to register and send information about events

CyberWay implements [Event Engine][5] that sends information about events occurring inside smart contracts to an external application.  

[1]: https://doxygen-cdt.cyberway.io/group__no-ricardians.html  
[2]: https://doxygen-cdt.cyberway.io/group__multiindex.html  
[3]: https://doxygen-cdt.cyberway.io/group__action.html  
[4]: https://doxygen-cdt.cyberway.io/group__domaincppapi.html  
[5]: https://docs.cyberway.io/devportal/event_engine  
