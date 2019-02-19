#include <eosiolib/ram.hpp>

#include <secram_dup_confirm.hpp>

ACTION secram_dup_confirm::approveram( name user, std::vector<name> contracts) {
    print_current_provider(user);

    eosio::print("Confirm ram by itself\n");
    confirm_approve_ram(user, contracts);

    eosio::print("Ask main provider to confirm it again\n");
    request_parent_to_approve(user, contracts);
    verify_approved(user);
}

EOSIO_DISPATCH( secram_dup_confirm, (approveram)(verify))
