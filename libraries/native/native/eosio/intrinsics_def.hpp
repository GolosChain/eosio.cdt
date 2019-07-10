#pragma once

#include <eosio/action.h>
#include <eosio/chain.h>
#include <eosio/crypto.h>
#include <eosio/permission.h>
#include <eosio/print.h>
#include <eosio/privileged.h>
#include <eosio/system.h>
#include <eosio/transaction.h>
#include <eosio/types.h>

#include <type_traits>

namespace eosio { namespace native {
   template <typename... Args, size_t... Is>
   auto get_args_full(std::index_sequence<Is...>) {
       std::tuple<std::decay_t<Args>...> tup;
       return std::tuple<Args...>{std::get<Is>(tup)...};
   }

   template <typename R, typename... Args>
   auto get_args_full(R(Args...)) {
       return get_args_full<Args...>(std::index_sequence_for<Args...>{});
   }

   template <typename R, typename... Args>
   auto get_args(R(Args...)) {
       return std::tuple<std::decay_t<Args>...>{};
   }
   
   template <typename R, typename Args, size_t... Is>
   auto create_function(std::index_sequence<Is...>) {
      return std::function<R(typename std::tuple_element<Is, Args>::type ...)>{ 
         [](typename std::tuple_element<Is, Args>::type ...) { 
            eosio_assert(false, "unsupported intrinsic"); return (R)0;
         }
      };
   }   

#define INTRINSICS(intrinsic_macro) \
intrinsic_macro(set_proposed_producers) \
intrinsic_macro(get_blockchain_parameters_packed) \
intrinsic_macro(set_blockchain_parameters_packed) \
intrinsic_macro(is_privileged) \
intrinsic_macro(get_active_producers) \
intrinsic_macro(assert_recover_key) \
intrinsic_macro(recover_key) \
intrinsic_macro(assert_sha256) \
intrinsic_macro(assert_sha1) \
intrinsic_macro(assert_sha512) \
intrinsic_macro(assert_ripemd160) \
intrinsic_macro(sha1) \
intrinsic_macro(sha256) \
intrinsic_macro(sha512) \
intrinsic_macro(ripemd160) \
intrinsic_macro(check_transaction_authorization) \
intrinsic_macro(check_permission_authorization) \
intrinsic_macro(get_permission_last_used) \
intrinsic_macro(get_account_creation_time) \
intrinsic_macro(current_time) \
intrinsic_macro(publication_time) \
intrinsic_macro(read_action_data) \
intrinsic_macro(action_data_size) \
intrinsic_macro(current_receiver) \
intrinsic_macro(require_recipient) \
intrinsic_macro(require_auth) \
intrinsic_macro(require_auth2) \
intrinsic_macro(has_auth) \
intrinsic_macro(is_account) \
intrinsic_macro(prints) \
intrinsic_macro(prints_l) \
intrinsic_macro(printi) \
intrinsic_macro(printui) \
intrinsic_macro(printi128) \
intrinsic_macro(printui128) \
intrinsic_macro(printsf) \
intrinsic_macro(printdf) \
intrinsic_macro(printqf) \
intrinsic_macro(printn) \
intrinsic_macro(printhex) \
intrinsic_macro(read_transaction) \
intrinsic_macro(transaction_size) \
intrinsic_macro(expiration) \
intrinsic_macro(tapos_block_prefix) \
intrinsic_macro(tapos_block_num) \
intrinsic_macro(get_action) \
intrinsic_macro(send_inline) \
intrinsic_macro(send_context_free_inline) \
intrinsic_macro(send_deferred) \
intrinsic_macro(cancel_deferred) \
intrinsic_macro(get_context_free_data) 

#define CREATE_ENUM(name) \
   name,

#define GENERATE_TYPE_MAPPING(name) \
   struct __ ## name ## _types { \
      using deduced_full_ts = decltype(eosio::native::get_args_full(::name)); \
      using deduced_ts      = decltype(eosio::native::get_args(::name)); \
      using res_t           = decltype(std::apply(::name, deduced_ts{})); \
      static constexpr auto is = std::make_index_sequence<std::tuple_size<deduced_ts>::value>(); \
   };

#define GET_TYPE(name) \
   decltype(create_function<eosio::native::intrinsics::__ ## name ## _types::res_t, \
         eosio::native::intrinsics::__ ## name ## _types::deduced_full_ts>(eosio::native::intrinsics::__ ## name ## _types::is)),

#define REGISTER_INTRINSIC(name) \
   create_function<eosio::native::intrinsics::__ ## name ## _types::res_t, \
         eosio::native::intrinsics::__ ## name ## _types::deduced_full_ts>(eosio::native::intrinsics::__ ## name ## _types::is),

}} //ns eosio::native
