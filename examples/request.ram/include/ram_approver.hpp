#pragma once
#include <eosiolib/eosio.hpp>
using namespace eosio;

CONTRACT ram_approver : public contract {
public:
   using contract::contract;

   void approve(name user, std::vector<name> contracts);
};
