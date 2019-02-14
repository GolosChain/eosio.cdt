/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <vector>

#include <eosiolib/ram.h>

#include <eosiolib/eosio.hpp>


namespace eosio {

   /**
    * @defgroup ram RAM API
    * @brief Defines API for interacting with ram providing
    *
    */

    /**
    *  @brief returns current ram provider.
    */
    inline eosio::name get_ram_provider(eosio::name user) {
        return eosio::name(::get_ram_provider(user.value));
    }

    /**
    *  @brief Confirm to provide ram.
    */
    inline void confirm_approve_ram(eosio::name user, const std::vector<eosio::name>& contracts) {
        ::confirm_approve_ram(user.value, reinterpret_cast<const capi_name *>(&contracts.data()->value), contracts.size());
    }

   ///@ } bandwith c api
}
