#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/bandwith.hpp>

class [[eosio::contract]] second_provider : public eosio::contract {
public:
  second_provider( eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds) : eosio::contract(receiver, code, ds) {}

  [[eosio::action]] void approvebw(eosio::name account) {
        eosio::print("Secondary provider\n");
        eosio::print("  Before set_bw_limit\n");
        eosio::print("      net limit \'", get_bw_net_limit(account), "\' cpu limit  \'", get_bw_cpu_limit(account), "\'\n");
        eosio::print("      is confirmed \'", is_provided_bw_confirmed(account), "\'\n");

        set_bw_limits(account, 200, 200);

        eosio::print("  After set_bw_limit\n");

        eosio::print("      net limit: ", get_bw_net_limit(account), "\' cpu limit  \'", get_bw_cpu_limit(account), "\'\n");
        eosio::print("      is confirmed: ", is_provided_bw_confirmed(account), "\n");

        eosio::print("Now calling the inline action of the main provider\n");

        eosio::action(
            eosio::permission_level{get_self(), "active"_n},
            "mainpw"_n,
            "approvebw"_n,
            account
        ).send();
  }

private:
};

EOSIO_DISPATCH( second_provider, (approvebw))
