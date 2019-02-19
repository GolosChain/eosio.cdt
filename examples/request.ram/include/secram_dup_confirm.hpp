#pragma once
#include <sec_base_provider.hpp>

using namespace eosio;

CONTRACT secram_dup_confirm : public sec_base_provider {
public:
    using sec_base_provider::sec_base_provider;

    ACTION approveram( name user, std::vector<name> contracts);
};
