#pragma once
#include <eosiolib/archive.h>
#include <eosiolib/datastream.hpp>
#include <eosiolib/serialize.hpp>

#include <boost/preprocessor/variadic/size.hpp>
#include <boost/preprocessor/variadic/to_tuple.hpp>
#include <boost/preprocessor/tuple/enum.hpp>
#include <boost/preprocessor/facilities/overload.hpp>

namespace eosio {

   /**
    * @defgroup archivecppapi Event C++ API
    * @ingroup archiveapi
    * @brief Defines type-safe C++ wrappers for saving and looking up archive records
    *
    * @note There are some methods from the @ref archivecapi that can be used directly from C++
    *
    * @{
    */

   /**
    * Save the archive record in block log
    *
    * @brief Save the archive record in block log
    * @tparam T - type of data for archive record
    * @param value - data for archive record
    * @return archive record id
    */
   template<typename T>
   uint64_t save_record(T &&value) {
      std::vector<char> data(pack(std::forward<T>(value)));
      return ::save_record(data.data(), data.size());
   }

   /**
    * Retrieve the unpacked data as T from archive record
    *
    * @brief Retrieve the unpacked data as T from archive record
    * @tparam T - expected type of data
    * @param rec_id - archive record id
    * @param code - contract that place this archive record
    * @param value - place for archive record value
    * @return true if archive record is found
    */
   template<typename T>
   bool lookup_record(uint64_t rec_id, name code, T& value) {
      constexpr size_t max_stack_buffer_size = 512;
      int size = ::lookup_record(rec_id, code.value, nullptr, 0);
      if( size == -1) {
         return false;
      }

      char* buffer = (char*)(max_stack_buffer_size < size ? malloc(size) : alloca(size));
      ::lookup_record(rec_id, code.value, buffer, size);

      value = unpack<T>(buffer, size);
      if( max_stack_buffer_size < size ) {
         free(buffer);
      }

      return true;
   }

   ///@} eventcpp api
   ///@}
}
