#pragma once

#include "types.hpp"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t scope_t;
typedef int32_t  cursor_t;

typedef uint64_t primary_key_t;
static constexpr primary_key_t end_primary_key = static_cast<primary_key_t>(-1);
static constexpr primary_key_t unset_primary_key = static_cast<primary_key_t>(-2);

__attribute__((eosio_wasm_import))
cursor_t chaindb_begin(capi_name code, scope_t scope, capi_name table, capi_name index);
__attribute__((eosio_wasm_import))
cursor_t chaindb_end(capi_name code, scope_t scope, capi_name table, capi_name index);
__attribute__((eosio_wasm_import))
cursor_t chaindb_lower_bound(capi_name code, scope_t scope, capi_name table, capi_name index, void* key, int32_t);
__attribute__((eosio_wasm_import))
cursor_t chaindb_lower_bound_pk(capi_name code, scope_t scope, capi_name table, primary_key_t);
__attribute__((eosio_wasm_import))
cursor_t chaindb_upper_bound(capi_name code, scope_t scope, capi_name table, capi_name index, void* key, int32_t);
__attribute__((eosio_wasm_import))
cursor_t chaindb_upper_bound_pk(capi_name code, scope_t scope, capi_name table, primary_key_t);
__attribute__((eosio_wasm_import))
cursor_t chaindb_locate_to(capi_name code, scope_t scope, capi_name table, capi_name index, primary_key_t, void* key, int32_t);
__attribute__((eosio_wasm_import))
cursor_t chaindb_clone(capi_name code, cursor_t);

__attribute__((eosio_wasm_import))
void chaindb_close(capi_name code, cursor_t);

__attribute__((eosio_wasm_import))
primary_key_t chaindb_current(capi_name code, cursor_t);
__attribute__((eosio_wasm_import))
primary_key_t chaindb_next(capi_name code, cursor_t);
__attribute__((eosio_wasm_import))
primary_key_t chaindb_prev(capi_name code, cursor_t);

__attribute__((eosio_wasm_import))
int32_t chaindb_datasize(capi_name code, cursor_t);
__attribute__((eosio_wasm_import))
primary_key_t chaindb_data(capi_name code, cursor_t, void* data, int32_t size);
__attribute__((eosio_wasm_import))
int32_t chaindb_service(capi_name code, cursor_t, void* data, int32_t size);

__attribute__((eosio_wasm_import))
primary_key_t chaindb_available_primary_key(capi_name code, scope_t scope, capi_name table);

__attribute__((eosio_wasm_import))
int32_t chaindb_insert(capi_name code, scope_t scope, capi_name, capi_name payer, primary_key_t, void* data, int32_t);
__attribute__((eosio_wasm_import))
int32_t chaindb_update(capi_name code, scope_t scope, capi_name table, capi_name payer, primary_key_t, void* data, int32_t);
__attribute__((eosio_wasm_import))
int32_t chaindb_delete(capi_name code, scope_t scope, capi_name table, capi_name payer, primary_key_t);

__attribute__((eosio_wasm_import))
void chaindb_ram_state(capi_name code, scope_t scope, capi_name table, primary_key_t, int32_t);

#ifdef __cplusplus
}
#endif
