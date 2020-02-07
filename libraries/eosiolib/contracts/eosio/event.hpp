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
    * @ingroup contracts
    * @ingroup eventapi
    * @brief Defines type-safe C++ wrappers for sending events
    *
    * @note The @ref eventcppapi contains several methods taken from the eventcapi group that can be used directly from C++.
    *
    * @ingroup eventcppapi
    *
    * @{
    */

   /**
    * CyberWay implements [Event Model](https://docs.cyberway.io/devportal/event_engine) which provides sending information about events occurring inside smart contracts to an external application. There is the packed representation of an event, including 
    * meta-data about the authorization levels.
    *
    * @brief Packed representation of an event.
    */
   struct event {
      /**
       * @brief Name of the account that the event is intended for
       */
      name                       account;

      /**
       * @brief Name of the event.
       */
      name                       name;

      /**
       * @brief Payload data processed by code.
       */
      std::vector<char>               data;

      /**
       * @brief By default, a new event object will be created by the Default Constructor.
       */
      event() = default;

      /**
       * @brief This method constructs a new event object, the execution of which allows a user to accurately reproduce the event with the given permission, event receiver, event name, event struct.
       * @tparam T - Type of the event struct, must be serializable by @a pack(...)
       * @param a - Name of the account that this event is intended for (event receiver)
       * @param n - Name of the event
       * @param value - The event struct that will be serialized via @a pack into data
       */
      template<typename T>
      event( struct name a, struct name n, T&& value )
      :account(a), name(n), data(eosio::pack(std::forward<T>(value))) {}

      EOSLIB_SERIALIZE( event, (account)(name)(data) )

      /**
       * After an event object is created, it is serialized and sent to external components (to message broker [NATS](https://docs.cyberway.io/devportal/event_engine#nats-message-broker)).
       *
       * @brief This method sends the event as inline event.
       */
      void send() const {
         auto serialize = pack(*this);
         internal_use_do_not_use::send_event(serialize.data(), serialize.size());
      }

      /**
       * On receiving side, the data packet is deserialized to obtain the object and components of structure.
       *
       * @brief This method retrieves the unpacked data as T.
       * @tparam T - Expected data type
       * @return the event data.
       */
      template<typename T>
      T data_as() {
         return eosio::unpack<T>( &data[0], data.size() );
      }

   };

   ///@} eventcpp api
   ///@}
}
