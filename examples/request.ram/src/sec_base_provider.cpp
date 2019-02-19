#include <eosiolib/ram.hpp>

#include <sec_base_provider.hpp>

ACTION sec_base_provider::verify(eosio::name user) {
      eosio::print("The verify action of secondary provider is called\n");

      eosio::print("Now ram provider for the user \'", user, "\' running \'", _self, "\' contract is \'", get_ram_provider(user), "\'\n");
}

void sec_base_provider::print_current_provider(eosio::name user) {
    eosio::print("Secondary provider\n");
    eosio::print("  Current \'", user, "\' ram provider: \'", get_ram_provider(user), "\' for the contract is \'", _self, "\'\n");
}


void sec_base_provider::request_parent_to_approve(eosio::name user, const std::vector<eosio::name>& contracts) {
    eosio::print("Send an inline action to the main provider\n");
    eosio::action(
        eosio::permission_level{get_self(), "active"_n},
        "main.pw"_n,
        "approveram"_n,
        std::make_tuple(user, contracts)
    ).send();
}


void sec_base_provider::verify_approved(eosio::name user) {
    eosio::print("Send an inline action to myself\n");
    eosio::action(
        eosio::permission_level{get_self(), "active"_n},
        get_self(),
        "verify"_n,
        user
    ).send();
}


