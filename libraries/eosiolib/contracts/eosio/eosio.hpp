/**
 *  @file
 *  @copyright defined in eos/LICENSE
 *  @copyright defined in cyberway/LICENSE
 */
#pragma once
#include "action.hpp"
#include "../../core/eosio/print.hpp"
#include "multi_index.hpp"
#include "dispatcher.hpp"
#include "contract.hpp"

#ifndef EOSIO_NATIVE
static_assert( sizeof(long) == sizeof(int), "unexpected size difference" );
#endif

/**
 * @defgroup modules Modules
 * @brief This section contains description of the source code modules.
 */

/**
 * @defgroup contracts Contracts API
 * @ingroup modules
 * @brief C++ Contracts API for chain-dependent smart-contract functionality.
 */

/**
 * @defgroup core Core API
 * @ingroup modules
 * @brief C++ Core API for chain-agnostic smart-contract functionality.
 */

/**
 * @defgroup types Types
 * @ingroup modules
 * @brief C++ Types API for data layout of data-structures available for the EOSIO platform.
 */
