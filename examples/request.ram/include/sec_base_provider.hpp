#pragma once
#include <eosiolib/eosio.hpp>

CONTRACT sec_base_provider : public eosio::contract {
public:
   using eosio::contract::contract;

   ACTION verify(eosio::name user);

   void print_current_provider(eosio::name user);

   void request_parent_to_approve(eosio::name user, const std::vector<eosio::name>& contracts);

   void verify_approved(eosio::name user);
};
