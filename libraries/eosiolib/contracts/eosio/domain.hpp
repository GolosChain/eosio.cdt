#pragma once
#include "../../core/eosio/name.hpp"
#include <string>

namespace eosio {
   namespace internal_use_do_not_use {
      extern "C" {

          __attribute__((eosio_wasm_import))
          int32_t is_domain(const char* domain);

          __attribute__((eosio_wasm_import))
          int32_t is_username(uint64_t scope, const char* username);

          __attribute__((eosio_wasm_import))
          uint64_t get_domain_owner(const char* domain);

          __attribute__((eosio_wasm_import))
          uint64_t resolve_domain(const char* domain);

          __attribute__((eosio_wasm_import))
          uint64_t resolve_username(uint64_t scope, const char* username);
      }
   }

using domain_name = std::string;
using username = std::string;

/**
 * @defgroup domaincppapi Domain C++ API
 * @ingroup contracts
 * @ingroup domainapi
 * @brief Defines API for interacting with domain/username related intrinsics
 *
 * @{
 */

/**
 *  @brief This method verifies that specified @a domain is an existing domain name in the system.
 *  @param domain - domain name to check
 *  @return @a true if the specified domain name exists in the system, or @a false otherwise.
 */
inline bool is_domain(const domain_name& domain) {
   return internal_use_do_not_use::is_domain(domain.c_str());
}

/**
 *  @brief This method verifies that specified @a username is an existing username in given scope.
 *  @param scope - account, at which @a scope username registered
 *  @param username - username to check
 *  @return @a true if the scope is available to account @a username, or @a false otherwise.
 */
inline bool is_username(name scope, const username& username) {
   return internal_use_do_not_use::is_username(scope.value, username.c_str());
}

/**
 *  @brief This method is used to get an owner of specified domain name. Throws if not found.
 *  @param domain - domain name to get owner
 *  @return the account which is current owner of given domain name.
 */
inline name get_domain_owner(const domain_name& domain) {
   return name{internal_use_do_not_use::get_domain_owner(domain.c_str())};
}

/**
 *  @brief This method is used to get an account which is currently linked to specified domain name. Throws if not found.
 *  @param domain - domain name to check
 *  @return the account which is currently linked to domain name. It can be "0" if domain unlinked.
 */
inline name resolve_domain(const domain_name& domain) {
   return name{internal_use_do_not_use::resolve_domain(domain.c_str())};
}

/**
 *  @brief This method is used to get an account which maps to specified username. Throws if not found.
 *  @param scope - account at which specified @a scope username registered
 *  @param username - username to check
 *  @return the account which maps to username in scope.
 */
inline name resolve_username(name scope, const username& username) {
   return name{internal_use_do_not_use::resolve_username(scope.value, username.c_str())};
}


///@ } domaincppapi
}
