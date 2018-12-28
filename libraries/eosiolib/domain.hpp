#pragma once
#include <eosiolib/domain.h>
#include <eosiolib/name.hpp>
#include <string>

namespace eosio {

using domain_name = std::string;
using username = std::string;

/**
 * @defgroup domaincppapi Domain C++ API
 * @ingroup domainapi
 * @brief Defines API for interacting with domain/username related intrinsics
 *
 * @{
 */


/**
 *  Verifies that @ref domain is an existing domain name.
 *  @param domain - domain name to check
 */
inline bool is_domain(const domain_name& domain) {
    return ::is_domain(domain.c_str());
}

/**
 *  Verifies that @ref username is an existing username in given scope.
 *  @param scope - account, at which "scope" username registered
 *  @param username - username to check
 */
inline bool is_username(name scope, const username& username) {
    return ::is_username(scope.value, username.c_str());
}

/**
 *  Get an owner of the given domain name. Throws if not found.
 *  @param domain - domain name to get owner
 *  @return the account which is current owner of given domain name
 */
inline name get_domain_owner(const domain_name& domain) {
    return name{::get_domain_owner(domain.c_str())};
}

/**
 *  Get an account, which is currently linked to given domain name. Throws if not found.
 *  @param domain - domain name to check
 *  @return the account which is currently linked to domain name. Can be 0 if domain unlinked
 */
inline name resolve_domain(const domain_name& domain) {
    return name{::resolve_domain(domain.c_str())};
}

/**
 *  Get an account, which maps to given username. Throws if not found.
 *  @param scope - account, at which "scope" username registered
 *  @param username - username to check
 *  @return the account which is maps to username in scope
 */
inline name resolve_username(name scope, const username& username) {
    return name{::resolve_username(scope.value, username.c_str())};
}


///@ } domaincppapi
}
