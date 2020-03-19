#pragma once
#include "types.h"

extern "C" {
/**
 * @defgroup domainapi Domain API
 * @ingroup contractdev
 * @brief Defines API for interacting with domain/username related intrinsics
 *
 */

/**
 * @defgroup domaincapi Domain C API
 * @ingroup domainapi
 * @brief Defines API for interacting with domain/username related intrinsics
 *
 * @{
 */


/**
 *  This method verifies that @a domain is an existing domain name.
 *  @param domain - domain name to check, a null terminated string
 */
__attribute__((eosio_wasm_import))
int32_t is_domain(const char* domain);

/**
 *  This method verifies that specified @a username is an existing username in given scope.
 *  @param scope - account, at which @a scope username registered
 *  @param username - username to check, a null terminated string
 */
__attribute__((eosio_wasm_import))
int32_t is_username(capi_name scope, const char* username);

/**
 *  This method is used to get an owner of specified domain name. Throws if not found.
 *  @param domain - domain name to get owner, a null terminated string
 *  @return the account which is current owner of given domain name.
 */
__attribute__((eosio_wasm_import))
capi_name get_domain_owner(const char* domain);

/**
 *  This method is used to get an account which is currently linked to specified domain name. Throws if not found.
 *  @param domain - domain name to check, a null terminated string
 *  @return the account which is currently linked to domain name. It can be "0" if domain unlinked.
 */
__attribute__((eosio_wasm_import))
capi_name resolve_domain(const char* domain);

/**
 *  This method is used to get an account which maps to specified username. Throws if not found.
 *  @param scope - account at which "scope" username registered
 *  @param username - username to check, a null terminated string
 *  @return the account which maps to username in scope
 */
__attribute__((eosio_wasm_import))
capi_name resolve_username(capi_name scope, const char* username);


///@ } domaincapi
}
