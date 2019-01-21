#pragma once
#include <eosiolib/system.h>

extern "C" {
   /**
    * @defgroup eventapi Action API
    * @ingroup contractdev
    * @brief Defines API for sending events
    *
    */

   /**
    * @defgroup eventcapi Action C API
    * @ingroup eventapi
    * @brief Defines API for sending events
    *
    *
    * A CyberWay.IO event has the following abstract structure:
    *
    * ```
    *   struct event {
    *     capi_name  account_name; // the contract defining the primary code to execute for code/type
    *     capi_name  event_name; // the event to be sended
    *     bytes data; // opaque data processed by code
    *   };
    * ```
    */

   /**
    *  Send an inline action in the context of this action's parent transaction
    *
    *  @param serialized_action - serialized action
    *  @param size - size of serialized action in bytes
    *  @pre `serialized_action` is a valid pointer to an array at least `size` bytes long
    */
   void send_event(char *serialized_action, size_t size);

   ///@ } actioncapi
}
