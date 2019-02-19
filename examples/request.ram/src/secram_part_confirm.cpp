#include <eosiolib/ram.h>

#include <secram_part_confirm.hpp>

ACTION secram_part_confirm::approveram( name user, std::vector<name> contracts) {
    print_current_provider(user);

    eosio::print("Confirm only part of the contracts\n");

    contracts.pop_back();
    request_parent_to_approve(user, contracts);
    verify_approved(user);
}

EOSIO_DISPATCH( secram_part_confirm, (approveram)(verify))
