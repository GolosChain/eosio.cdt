/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once
#include <eosiolib/types.h>

namespace eosio {

   /**
    * @defgroup bandwith Bandwith API
    * @brief Defines API for interacting with bandwith intrinsics
    *
    */

   /**
    * @defgroup bandwith Bandwith C API
    * @ingroup bandwith
    * @brief Defines API for interacting with bandwith intrinsics
    *
    * @{
    */

    /**
    *  Returns current cpu bandwith value
    *  @brief Get cpu time avalable for transaction
    *  @return time
    */
    inline uint64_t get_bw_cpu_limit(eosio::name account) {
        return ::get_bw_cpu_limit(account.value);
    }

   /**
   *  Returns current net bandwith value
   *  @brief Get network limit avalable for transaction
   *  @return bytes
   */
    inline uint64_t get_bw_net_limit(eosio::name account) {
        return ::get_bw_net_limit(account.value);
    }

   /**
   *  Returns true if any provider confirmed to give his bandwith
   *  @brief Get confirmation status
   *  @return true if provider confirm to give his bandwith
   */
    inline bool is_provided_bw_confirmed(eosio::name account) {
        return ::is_provided_bw_confirmed(account.value);
    }


   /**
   *  @brief Set valuest of the banwith this contract is ready to provide
   */
    inline void set_bw_limits(eosio::name account, uint64_t net, uint64_t cpu) {
        ::set_bw_limits(account.value, net, cpu);
    }


   /**
   *  @brief Confirm to provide bandwith. Bandwith values couldn't be changed after this operation.
   */
    inline void confirm_bw_limits(eosio::name account) {
        ::confirm_bw_limits(account.value);
    }

   ///@ } bandwith c api

}
