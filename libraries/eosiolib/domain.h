#pragma once
#include <eosiolib/types.h>

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
 *  Verifies that @ref domain is an existing domain name.
 *  @param domain - domain name to check, a null terminated string
 */
bool is_domain(const char* domain);

/**
 *  Verifies that @ref username is an existing username in given scope.
 *  @param scope - account, at which "scope" username registered
 *  @param username - username to check, a null terminated string
 */
bool is_username(capi_name scope, const char* username);

/**
 *  Get an owner of the given domain name. Throws if not found.
 *  @param domain - domain name to get owner, a null terminated string
 *  @return the account which is current owner of given domain name
 */
capi_name get_domain_owner(const char* domain);

/**
 *  Get an account, which is currently linked to given domain name. Throws if not found.
 *  @param domain - domain name to check, a null terminated string
 *  @return the account which is currently linked to domain name. Can be 0 if domain unlinked
 */
capi_name resolve_domain(const char* domain);

/**
 *  Get an account, which maps to given username. Throws if not found.
 *  @param scope - account, at which "scope" username registered
 *  @param username - username to check, a null terminated string
 *  @return the account which is maps to username in scope
 */
capi_name resolve_username(capi_name scope, const char* username);


///@ } domaincapi
}
