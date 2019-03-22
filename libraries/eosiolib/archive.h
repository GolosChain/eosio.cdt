#pragma once
#include <eosiolib/system.h>

extern "C" {
   /**
    * @defgroup archiveapi Action API
    * @ingroup contractdev
    * @brief Defines API for save and lookup archive records
    *
    */

   /**
    * @defgroup archivecapi Action C API
    * @ingroup archiveapi
    * @brief Defines API for save and lookup archive records
    */

   /**
    * Save the archive record in block log
    *
    * @brief Save the archive record in block log
    * @param data - data for archive record
    * @param size - data size for archive record
    * @return archive record id
    */
   uint64_t save_record(const void *data, size_t size);

   /**
    * Retrieve the data from archive record
    *
    * @brief Retrieve the data from archive record
    * @param rec_id - archive record id
    * @param code - contract that place this archive record
    * @param buffer - buffer for archive record data
    * @param buffer_size - size of buffer for archive record data
    * @return number of copied bytes (if size is not zero) or length of archive
    *         record (if size is zero). Return -1 if archive record is not found. 
    */
   int lookup_record(uint64_t rec_id, capi_name code, void* buffer, size_t buffer_size);

   ///@ } actioncapi
}
