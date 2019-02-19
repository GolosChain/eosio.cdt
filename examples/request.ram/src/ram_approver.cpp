#include <string>

#include <eosiolib/ram.hpp>
#include <mainrampw.hpp>

void ram_approver::approve( name user, std::vector<name> contracts) {
    std::string contracts_list;

    for (const auto& contract : contracts) {
        contracts_list += " \'" + contract.to_string() + "\',";
    }
    contracts_list.erase(contracts_list.size() - 1);

    eosio::print(_self, " will provide memory for the contracts", contracts_list,". Current is ", get_ram_provider(user), " \n");
    confirm_approve_ram(user, contracts);
    eosio::print("Now provider for the contract \'",_self ,"\' is ", get_ram_provider(user), "\n");
}

