#include <eosiolib/ram.h>

#include <secram_no_confirm.hpp>

ACTION secram_no_confirm::approveram( name user, std::vector<name>) {
    print_current_provider(user);
    eosio::print("Don not confirm ram\n");

    verify_approved(user);
}

EOSIO_DISPATCH( secram_no_confirm, (approveram)(verify))
