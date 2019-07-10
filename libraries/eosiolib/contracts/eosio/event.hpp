#pragma once
#include "../../core/eosio/serialize.hpp"
#include "../../core/eosio/datastream.hpp"
#include "../../core/eosio/name.hpp"

#include <vector>

#include <boost/preprocessor/variadic/size.hpp>
#include <boost/preprocessor/variadic/to_tuple.hpp>
#include <boost/preprocessor/tuple/enum.hpp>
#include <boost/preprocessor/facilities/overload.hpp>

namespace eosio {
   namespace internal_use_do_not_use {
      extern "C" {

         __attribute__((eosio_wasm_import))
         void send_event(char *serialized_action, int32_t size);
      }
   }

   /**
    * @defgroup eventcppapi Event C++ API
    * @ingroup eventapi
    * @brief Defines type-safe C++ wrappers for sending events
    *
    * @note There are some methods from the @ref eventcapi that can be used directly from C++
    *
    * @{
    */

   /**
    * This is the packed representation of an event along with
    * meta-data about the authorization levels.
    *
    * @brief Packed representation of an event
    */
   struct event {
      /**
       * Name of the account the event is intended for
       *
       * @brief Name of the account the event is intended for
       */
      name                       account;

      /**
       * Name of the event
       *
       * @brief Name of the event
       */
      name                       name;

      /**
       * Payload data
       *
       * @brief Payload data
       */
      std::vector<char>               data;

      /**
       * Default Constructor
       *
       * @brief Construct a new event object
       */
      event() = default;

      /**
       * Construct a new event object with the given event struct
       *
       * @brief Construct a new event object with the given permission, event receiver, event name, event struct
       * @tparam T  - Type of event struct, must be serializable by `pack(...)`
       * @param auth - The permissions that authorizes this event
       * @param a -  The name of the account this event is intended for (event receiver)
       * @param n - The name of the event
       * @param value - The event struct that will be serialized via pack into data
       */
      template<typename T>
      event( struct name a, struct name n, T&& value )
      :account(a), name(n), data(eosio::pack(std::forward<T>(value))) {}

      EOSLIB_SERIALIZE( event, (account)(name)(data) )

      /**
       * Send the event as inline event
       *
       * @brief Send the event as inline event
       */
      void send() const {
         auto serialize = pack(*this);
         internal_use_do_not_use::send_event(serialize.data(), serialize.size());
      }

      /**
       * Retrieve the unpacked data as T
       *
       * @brief Retrieve the unpacked data as T
       * @tparam T expected type of data
       * @return the event data
       */
      template<typename T>
      T data_as() {
         return eosio::unpack<T>( &data[0], data.size() );
      }

   };

   ///@} eventcpp api
   ///@}
}
