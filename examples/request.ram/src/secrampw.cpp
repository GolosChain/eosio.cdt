#include <secrampw.hpp>

ACTION secrampw::approveram( name user, std::vector<name> contracts) {
    print_current_provider(user);
    request_parent_to_approve(user, contracts);
    verify_approved(user);
}

EOSIO_DISPATCH( secrampw, (approveram)(verify))
