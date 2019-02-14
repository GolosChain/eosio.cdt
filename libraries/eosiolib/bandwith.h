/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once
#include <eosiolib/types.h>

extern "C" {

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
   uint64_t get_bw_cpu_limit(capi_name account);

   /**
   *  Returns current net bandwith value
   *  @brief Get network limit avalable for transaction
   *  @return bytes
   */
   uint64_t get_bw_net_limit(capi_name account);

   /**
   *  Returns true if any provider confirmed to give his bandwith
   *  @brief Get confirmation status
   *  @return true if provider confirm to give his bandwith
   */
   bool is_provided_bw_confirmed(capi_name account);


   /**
   *  @brief Set valuest of the banwith this contract is ready to provide
   */
   void set_bw_limits(capi_name account, uint64_t net, uint64_t cpu);


   /**
   *  @brief Confirm to provide bandwith. Bandwith values couldn't be changed after this operation.
   */
   void confirm_bw_limits(capi_name account);

   ///@ } bandwith c api

}
