#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/bandwith.hpp>

class [[eosio::contract]] main_provider : public eosio::contract {
public:
  main_provider( eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds) : eosio::contract(receiver, code, ds) {}

  [[eosio::action]] void approvebw(eosio::name account) {
      eosio::print("Main provider\n");
      eosio::print("  Before set_bw_limit\n");
      eosio::print("      net limit \'", get_bw_net_limit(account), "\' cpu limit  \'", get_bw_cpu_limit(account), "\'\n");
      eosio::print("      is confirmed \'", is_provided_bw_confirmed(account), "\'\n");

      set_bw_limits(account, 300, 300);

      eosio::print("  After set_bw_limit\n");

      eosio::print("      net limit: ", get_bw_net_limit(account), "\' cpu limit  \'", get_bw_cpu_limit(account), "\'\n");
      eosio::print("      is confirmed: ", is_provided_bw_confirmed(account), "\n");

      confirm_bw_limits(account);

      eosio::print("  After confirm_bw_limits\n");

      eosio::print("      is confirmed: ", is_provided_bw_confirmed(account), "\n");
  }
};

EOSIO_DISPATCH( main_provider, (approvebw))

