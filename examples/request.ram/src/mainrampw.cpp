#include <mainrampw.hpp>

ACTION mainrampw::approveram( name user, std::vector<name> contracts) {
    eosio::print("Main provider will approve to provide ram\n");
    approve(user, contracts);
}

EOSIO_DISPATCH(mainrampw, (approveram))
