/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <eosiolib/types.h>

extern "C" {

   /**
    * @defgroup ram RAM API
    * @brief Defines API for interacting with ram providing
    *
    */

    /**
    *  @brief returns current ram provider.
    */
    capi_name get_ram_provider(capi_name user);

    /**
    *  @brief Confirm to provide ram.
    */
    void confirm_approve_ram(capi_name user, const capi_name* contracts, size_t count);

   ///@ } bandwith c api
}
