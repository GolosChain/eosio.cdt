#pragma once
#include <eosiolib/eosio.hpp>

#include <ram_approver.hpp>

using namespace eosio;

CONTRACT mainrampw : public ram_approver {
public:
   using ram_approver::ram_approver;

   ACTION approveram( name user, std::vector<name> contracts);
};
